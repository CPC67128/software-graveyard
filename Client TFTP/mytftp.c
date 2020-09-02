/*********************************
 *  Client TFTP  *  FUCHS Steve  *
 *********************************/

/***** Inclusions des diff�rentes librairies C *****/
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>

/***** Constantes du programme *****/
#define PDU_LENGTH 516
#define NMAX 5

/***** Structures de donn�es utilis�es *****/
struct PDU_RRQ
{
	u_short opcode;
	char msg[1];
};

struct PDU_RECV
{
	u_short opcode;
	u_short type;
	char msg[1];
};

struct PDU_ACK
{
	u_short opcode;
	u_short nbloc;
};

struct PDU_ERROR
{
	u_short opcode;
	u_short code;
	char msg[1];
};

/***** D�finitions des variables globales *****/
char paquetRRQ [PDU_LENGTH];
char paquetRecv [PDU_LENGTH];
char paquetACK [4];
char paquetERROR [PDU_LENGTH];
struct PDU_RRQ * pdu_rrq = (struct PDU_RRQ *) paquetRRQ;
struct PDU_RECV * pdu_recv = (struct PDU_RECV *) paquetRecv;
struct PDU_ACK * pdu_ack = (struct PDU_ACK *) paquetACK;
struct PDU_ERROR * pdu_error = (struct PDU_ERROR *) paquetERROR;
int tailleRRQ;
struct sockaddr_in adrForRRQ;
struct sockaddr_in adrFromRecv;
struct sockaddr_in save_adrFromRecv;
unsigned short portDest;
static char * mode = "octet";
int sockid;
int tailleAdrFromRecv;

/***** Prototypes des fonctions utilis�es *****/
void erreur(char * s);
char * extractFilename(char * file);
void createFile(char * file);
void bufferToFile(char * file, char * buffer, int taille);
struct hostent * getInfosServeur(char * source);
void afficherRRQ();
void afficherACK();
void creerRRQ(char * fichier);
int envoyerRRQ();
int envoyerACK();
int recevoir();
void envoyerERROR(int code, char * msg);

/***** Programme ex�cutable ******/
main(int argc, char ** argv)
{
	/* D�clarations des variables */
	char etat;
	char * destination;
	int nbCarSend, nbCarRcpt, retSelect, transfertOK, N;
	u_short tid, nbloc;
	fd_set readfs;
	struct hostent * infosServeur;
	struct timeval timeout;

	/* V�rification du nombre d'arguments */
	if (argc > 4 || argc < 3)
		erreur("Usage : mytftp <machine> <fichier> <fichier> [<port>]");

	/* Initialisation des variables */
	destination = extractFilename(argv[2]);
	pdu_ack->opcode = htons(4);
	pdu_error->opcode = htons(5);
	etat = 0;

	/* R�cup�ration des informations sur la machine serveur */
	if ((infosServeur = getInfosServeur(argv[1])) == NULL)
		erreur("ETAT 0.1 : La machine demand�e n'existe pas");

	/* Cr�ation du socket de communication */
	if ((sockid = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
		erreur("ETAT 0.1 : La cr�ation du socket a �chou�e");

	/* Cr�ation des informations sur la machine serveur */
	adrForRRQ.sin_family = AF_INET;
	memcpy((char *) &(adrForRRQ.sin_addr.s_addr), (char *) infosServeur->h_addr_list[0], 4);
	adrForRRQ.sin_port = htons((argc == 3) ? 0x45 : atoi(argv[3]));

	/* Cr�ation et envoi de la requ�te RRQ */
	creerRRQ(argv[2]);
	printf("ETAT 0.1 : Paquet RRQ : "); afficherRRQ(paquetRRQ);
	if ((nbCarSend = envoyerRRQ()) < tailleRRQ)
		erreur("ETAT 0.1 : Echec lors de l'envoi du paquet RRQ");

	/* Modifications des variables */
	nbloc = 1;
	N = 1;
	etat = 1;

	/* Boucle de r�ception des blocs */
	while (etat != 4)
	{
		/* Mise en place du select */
		timeout.tv_sec = 5; timeout.tv_usec = 0;
		FD_ZERO((fd_set *) &readfs);
		FD_SET(sockid, (fd_set *) &readfs);
		retSelect = select((sockid + 1), (fd_set *) &readfs, NULL, NULL, &timeout);

		/* ETAT 1 */
		if (etat == 1)
		{
			if (retSelect == 1)
			{
				/* R�ception du PDU */
				tailleAdrFromRecv = sizeof(adrFromRecv);
				if ((nbCarRcpt = recevoir()) < 4)
					erreur("ETAT 1   : Echec lors de la r�ception du paquet");
				pdu_recv->opcode = ntohs(pdu_recv->opcode);
				pdu_recv->type = ntohs(pdu_recv->type);

/* ETAT 1.1 */	if (pdu_recv->opcode == 3 && pdu_recv->type == 1 && nbCarRcpt == PDU_LENGTH)
				{
					printf("ETAT 1.1 : R�ception du DATA 1 de %d octets\n", PDU_LENGTH);

					/* Cr�ation du fichier de sortie et �criture des r�sultats */
					printf("ETAT 1.1 : Cr�ation du fichier : %s\n", destination);
					createFile(destination);
					bufferToFile(destination, paquetRecv + 4, PDU_LENGTH - 4);

					/* Emission de l'acquittement pour r�ception du bloc 1 */
					pdu_ack->nbloc = htons(pdu_recv->type);
					if ((nbCarSend = envoyerACK()) != 4)
						erreur("ETAT 1.1 : Echec lors de l'envoi du paquet d'acquittement du bloc 1");
					printf("ETAT 1.1 : Envoi de ACK 1 : "); afficherACK(paquetACK);

					/* Modifications des variables */
					nbloc = 2;
					N = 1;
					tid = ntohs(adrFromRecv.sin_port);
					memcpy((char *) &save_adrFromRecv, (char *) &adrFromRecv, sizeof adrFromRecv);
					etat = 2;
				}
/* ETAT 1.2 */	else if (pdu_recv->opcode == 3 && pdu_recv->type == 1 && nbCarRcpt < PDU_LENGTH)
				{
					printf("ETAT 1.2 : R�ception du DATA 1 de moins de %d octets\n", PDU_LENGTH);

					/* Cr�ation du fichier de sortie et �criture des r�sultats */
					printf("ETAT 1.2 : Cr�ation du fichier : %s\n", destination);
					createFile(destination);
					bufferToFile(destination, paquetRecv + 4, PDU_LENGTH - 4);

					/* Emission de l'acquittement pour r�ception du bloc 1 */
					pdu_ack->nbloc = htons(pdu_recv->type);
					if ((nbCarSend = envoyerACK()) != 4)
						erreur("ETAT 1.2 : Echec lors de l'envoi du paquet d'acquittement du bloc 1");
					printf("ETAT 1.2 : Envoi de ACK 1 : "); afficherACK(paquetACK);

					/* Modifications des variables */
					nbloc = 2;
					N = 1;
					tid = ntohs(adrFromRecv.sin_port);
					memcpy((char *) &save_adrFromRecv, (char *) &adrFromRecv, sizeof adrFromRecv);
					etat = 3;
				}
/* ETAT 1.5 */	else if (pdu_recv->opcode == 5)
				{
					printf("ETAT 1.5 : R�ception d'un PDU d'erreur apr�s l'envoi du PDU RRQ\n");

					/* Affichage de l'erreur */
					printf("ETAT 1.5 : Erreur : ");
					switch (pdu_recv->type)
					{
						case 1 : printf("Fichier introuvable\n"); break;
						case 2 : printf("Violation d'acc�s\n"); break;
						case 3 : printf("Disque plein ou allocation impossible\n"); break;
						case 4 : printf("Op�ration TFTP non autoris�e\n"); break;
						case 5 : printf("Identificateur de transfert inconnu\n"); break;
						case 6 : printf("Le fichier existe d�j�\n"); break;
						default : printf("%s\n", paquetRecv + 4); break;
					}

					/* Modifications des variables */
					transfertOK = 0;
					etat = 4;
				}
/* ETAT 1.6 */	else if (((pdu_recv->opcode == 3 && pdu_recv->type != 1) ||
						 (pdu_recv->opcode != 3 && pdu_recv->opcode != 5)) && N < NMAX)
				{
					printf("ETAT 1.6 : R�ception d'un PDU DATA != 1 ou PDU TFTP * - N < NMAX\n");

					/* Emission d'un paquet d'erreur */
					printf("ETAT 1.6 : Emission d'un paquet d'erreur de code 4\n");
					envoyerERROR(4, "op�ration TFTP non autoris�e");

					/* Envoi du message RRQ */
					printf("ETAT 1.6 : Envoi du paquet RRQ : "); afficherRRQ(paquetRRQ);
					if ((nbCarSend = envoyerRRQ()) != tailleRRQ)
						erreur("ETAT 1.6 : Echec lors de l'envoi du paquet RRQ");

					/* Modifications des variables */
					N++;
					etat = 1;
				}
/* ETAT 1.7 */	else if (((pdu_recv->opcode == 3 && pdu_recv->type != 1) ||
						 (pdu_recv->opcode != 3 && pdu_recv->opcode != 5)) && N == NMAX)
				{
					printf("ETAT 1.7 : R�ception d'un PDU DATA != 1 ou PDU TFTP * - N = NMAX\n");

					/* Modifications des variables */
					transfertOK = 0;
					etat = 4;
				}
			}
			else if (retSelect == 0)
			{
/* ETAT 1.3 */	if (N < NMAX)
				{
					printf("ETAT 1.3 : Tempo expir� - N < NMAX\n");

					/* Envoi du message RRQ */
					printf("ETAT 1.3 : Paquet RRQ : "); afficherRRQ(paquetRRQ);
					if ((nbCarSend = envoyerRRQ()) != tailleRRQ)
						erreur("ETAT 1.3 : Echec lors de l'envoi du paquet RRQ");

					/* Modifications des variables */
					N++;
				}
/* ETAT 1.4 */	else
				{
					printf("ETAT 1.4 : Tempo expir� - N = NMAX\n");

					/* Modifications des variables */
					transfertOK = 0;
					etat = 4;
				}
			}
			else
				erreur("La primitive select a renvoy�e une erreur");
		}
		/* ETAT 2 */
		else if (etat == 2)
		{
			/* Gestion du r�sultat du select */
			if (retSelect == 1)
			{
				/* R�ception du PDU */
				tailleAdrFromRecv = sizeof(adrFromRecv);
				if ((nbCarRcpt = recevoir()) < 4)
					erreur("ETAT 2   : Echec lors de la r�ception du paquet");
				pdu_recv->opcode = ntohs(pdu_recv->opcode);
				pdu_recv->type = ntohs(pdu_recv->type);

				/* Traduction des �venements du graphe d'�venements */
/* ETAT 2.8 */	if (ntohs(adrFromRecv.sin_port) != tid)
				{
					printf("ETAT 2.8 : R�ception d'un paquet d'un autre port\n");
					printf("ETAT 2.8 : Emission d'un paquet d'erreur de code 5\n");

					/* Emission d'un paquet d'erreur */
					envoyerERROR(5, "mauvais port");

					/* Modifications des variables */
					memcpy((char *) &adrFromRecv, (char *) &save_adrFromRecv, sizeof save_adrFromRecv);
					etat = 2;
				}
/* ETAT 2.1 */	else if (pdu_recv->opcode == 3 && pdu_recv->type == nbloc && nbCarRcpt == PDU_LENGTH)
				{
					printf("ETAT 2.1 : R�ception du PDU DATA %d de %d octets\n", nbloc, PDU_LENGTH);
					bufferToFile(destination, paquetRecv + 4, nbCarRcpt - 4);

					/* Emission de l'acquittement pour r�ception du bloc nbloc */
					pdu_ack->nbloc = htons(pdu_recv->type);
					if ((nbCarSend = envoyerACK()) != 4)
						erreur("ETAT 2.1 : Echec lors de l'envoi du paquet d'acquittement du bloc nbloc");
					printf("ETAT 2.1 : Envoi de ACK %d : ", nbloc); afficherACK(paquetACK);

					/* Modifications des variables */
					nbloc++;
					N = 1;
					etat = 2;
				}
/* ETAT 2.2 */	else if (pdu_recv->opcode == 3 && pdu_recv->type == nbloc && nbCarRcpt < PDU_LENGTH)
				{
					printf("ETAT 2.2 : R�ception du PDU DATA %d de moins de %d octets\n", nbloc, PDU_LENGTH);
					bufferToFile(destination, paquetRecv + 4, nbCarRcpt - 4);

					/* Emission de l'acquittement pour r�ception du bloc nbloc */
					pdu_ack->nbloc = htons(pdu_recv->type);
					if ((nbCarSend = envoyerACK()) != 4)
						erreur("ETAT 2.2 : Echec lors de l'envoi du paquet d'acquittement du bloc nbloc");
					printf("ETAT 2.2 : Envoi de ACK %d : ", nbloc); afficherACK(paquetACK);

					/* Modifications des variables */
					nbloc++;
					N = 1;
					etat = 3;
				}
/* ETAT 2.5 */	else if (pdu_recv->opcode == 5)
				{
					printf("ETAT 2.5 : R�ception d'un PDU d'erreur\n");

					/* Affichage de l'erreur */
					printf("ETAT 2.5 : ");
					switch (pdu_recv->type)
					{
						case 1 : printf("Fichier introuvable\n"); break;
						case 2 : printf("Violation d'acc�s\n"); break;
						case 3 : printf("Disque plein ou allocation impossible\n"); break;
						case 4 : printf("Op�ration TFTP non autoris�e\n"); break;
						case 5 : printf("Identificateur de transfert inconnu\n"); break;
						case 6 : printf("Le fichier existe d�j�\n"); break;
						default : printf("%s\n", paquetRecv + 4); break;
					}

					/* Modifications des variables */
					transfertOK = 0;
					etat = 4;
				}
/* ETAT 2.6 */	else if (pdu_recv->opcode == 3 && pdu_recv->type != nbloc && N < NMAX)
				{
					printf("ETAT 2.6 : R�ception d'un PDU DATA != %d - N < NMAX\n", nbloc);

					/* Emission de l'acquittement pour r�ception du bloc nbloc - 1 */
					pdu_ack->nbloc = htons(nbloc - 1);
					if ((nbCarSend = envoyerACK()) != 4)
						erreur("ETAT 2.6 : Echec lors de l'envoi du paquet d'acquittement du bloc nbloc - 1");
					printf("ETAT 2.6 : Envoi de ACK %d : ", nbloc - 1); afficherACK(paquetACK);

					/* Modifications des variables */
					N++;
					etat = 2;
				}
/* ETAT 2.7 */	else if (pdu_recv->opcode == 3 && pdu_recv->type != nbloc && N >= NMAX)
				{
					printf("ETAT 2.7 : R�ception d'un PDU DATA != %d - N = NMAX\n", nbloc);

					/* Modifications des variables */
					transfertOK = 0;
					etat = 4;
				}
			}
			else if (retSelect == 0)
			{
/* ETAT 2.3 */	if (N < NMAX)
				{
					/* Message de description de l'action */
					printf("ETAT 2.3 : Le temps est �coul� - N < NMAX\n");

					/* Emission de l'acquittement pour r�ception du bloc nbloc - 1 */
					pdu_ack->nbloc = htons(nbloc - 1);
					if ((nbCarSend = envoyerACK()) != 4)
						erreur("ETAT 2.3 : Echec lors de l'envoi du paquet d'acquittement du bloc nbloc - 1");
					printf("ETAT 2.3 : Envoi de ACK %d : ", nbloc - 1); afficherACK(paquetACK);

					/* Modifications des variables */
					N++;
					etat = 2;
				}
/* ETAT 2.4 */	else
				{
					printf("ETAT 2.4 : Le temps est �coul� - N = NMAX\n");

					/* Modifications des variables */
					transfertOK = 0;
					etat = 4;
				}
			}
			else
				erreur("La primitive select a renvoy�e une erreur");
		}
		/* ETAT 3 */
		else if (etat == 3)
		{
			/* Gestion du r�sultat du select */
			if (retSelect == 1)
			{
				/* R�ception du PDU */
				tailleAdrFromRecv = sizeof(adrFromRecv);
				if ((nbCarRcpt = recevoir()) < 4)
					erreur("ETAT 3   : Echec lors de la r�ception du paquet");
				pdu_recv->opcode = ntohs(pdu_recv->opcode);
				pdu_recv->type = ntohs(pdu_recv->type);

				/* Traduction des �venements du graphe d'�venements */
/* ETAT 3.5 */	if (ntohs(adrFromRecv.sin_port) != tid)
				{
					printf("ETAT 3.5 : R�ception d'un paquet d'un autre port\n");
					printf("ETAT 3.5 : Emission d'un paquet d'erreur de code 5\n");

					/* Emission d'un paquet d'erreur */
					envoyerERROR(5, "mauvais port");

					/* Modifications des variables */
					memcpy((char *) &adrFromRecv, (char *) &save_adrFromRecv, sizeof save_adrFromRecv);
					etat = 3;
				}
/* ETAT 3.2 */	else if (pdu_recv->opcode == 5)
				{
					printf("ETAT 3.2 : R�ception d'un PDU d'erreur\n");

					/* Affichage de l'erreur */
					printf("ETAT 3.2 : ");
					switch (pdu_recv->type)
					{
						case 1 : printf("Fichier introuvable\n"); break;
						case 2 : printf("Violation d'acc�s\n"); break;
						case 3 : printf("Disque plein ou allocation impossible\n"); break;
						case 4 : printf("Op�ration TFTP non autoris�e\n"); break;
						case 5 : printf("Identificateur de transfert inconnu\n"); break;
						case 6 : printf("Le fichier existe d�j�\n"); break;
						default : printf("%s\n", paquetRecv + 4); break;
					}

					/* Modifications des variables */
					transfertOK = 1;
					etat = 4;
				}
/* ETAT 3.3 */	else if (N < NMAX)
				{
					printf("ETAT 3.3 : R�ception d'un PDU DATA * - N < NMAX\n");

					/* Emission de l'acquittement pour r�ception du bloc nbloc - 1 */
					pdu_ack->nbloc = htons(nbloc - 1);
					if ((nbCarSend = envoyerACK()) != 4)
						erreur("ETAT 3.3 : Echec lors de l'envoi du paquet d'acquittement du bloc nbloc - 1");
					printf("ETAT 3.3 : Envoi de ACK %d : ", nbloc - 1); afficherACK(paquetACK);

					/* Modifications des variables */
					N++;
					etat = 3;
				}
/* ETAT 3.4 */	else if (N >= NMAX)
				{
					printf("ETAT 3.4 : R�ception d'un PDU DATA * - N = NMAX\n");

					/* Modifications des variables */
					transfertOK = 1;
					etat = 4;
				}
			}
			else if (retSelect == 0)
			{
/* ETAT 3.1 */	printf("ETAT 3.1 : Le temps de courtoisie est �coul�\n");

				/* Modifications des variables */
				transfertOK = 1;
				etat = 4;
			}
			else
				erreur("La primitive select a renvoy�e une erreur");
		}
	}
	printf("ETAT 4   : FIN DU TRANSFERT : %s\n", (transfertOK)?"OK":"NON OK");
	if (!transfertOK)
		if (unlink(destination) != 0)
			if (errno != ENOENT)
				erreur("Erreur lors de la suppression du fichier temporaire sur le disque");
}

/***** Fonctions du programme *****/

/* Proc�dure erreur du programme*/
void erreur(char * s)
{
	if (errno)	perror(s);
	else	fprintf(stderr, "%s\n", s);
	exit(1);
}

/* Proc�dure extractFilename qui r�cup�re le nom du fichier � �crire sur le disque */
char * extractFilename(char * file)
{
	char * filename = file + strlen(file);
	while (*filename != '/' && filename >= file)	filename--;
	if (filename < file)	filename = file; else filename++;
	return (filename);
}

/* Proc�dure de cr�ation du fichier de sortie */
void createFile(char * file)
{
	int fd = creat(file, 0666);
	if (fd < 0)		erreur("Erreur lors de la cr�ation du fichier sur le disque");
	if (close(fd))	erreur("Erreur lors de la fermeture du fichier sur le disque");
}

/* Proc�dure d'inscription d'un buffer d'octets dans un fichier donn� */
void bufferToFile(char * file, char * buffer, int taille)
{
	int fd;
	if ((fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666)) < 0)
		erreur("Erreur lors de l'ouverture du fichier pour �criture");
	if (write(fd, buffer, taille) != taille)
		erreur("Probl�me lors de l'�criture dans le fichier sur le disque");
	if (close(fd))
		erreur("Probl�me lors de la fermeture dans le fichier sur le disque");
}

/* Fonction getInfosServeur qui permet de r�cup�rer les informations d'une machine distante */
struct hostent * getInfosServeur(char * source)
{
	/* D�clarations des variables */
	unsigned long ip;
	struct hostent *infos = NULL;
	unsigned char *adresse;
	char *c_ip;

	/* Recherche des informations de la machine */ 
	if ((ip = inet_addr(source)) != -1)
	{
		assert(adresse = malloc(sizeof(unsigned long)));
		memcpy(adresse, &(ip), sizeof(unsigned long));
		infos = gethostbyaddr(adresse, 4, AF_INET);
	}
	else
		infos = gethostbyname(source);

	return infos;
}

/* Fonction afficherRRQ permettant d'aficher la requ�te RRQ */
void afficherRRQ()
{
	int x;
	for (x = 0; x < tailleRRQ; x++)
	{
		if (paquetRRQ[x]) printf("%c ", paquetRRQ[x]);
		else printf("\\0 ");
	}
	printf("\n");
}

/* Fonction afficherACK qui permet d'afficher le paquet d'acquittement qui a �t� envoy� */
void afficherACK()
{
	int x; printf("(");
	for (x = 0; x < 3; x++) printf("%x ", paquetACK[x]);
	printf("%x)\n", paquetACK[3]);
}

/* Fonction permettant de concevoir le paquet RRQ � envoyer */
void creerRRQ(char * fichier)
{
	tailleRRQ = 2 + strlen(fichier) + 1 + strlen(mode) + 1;
	pdu_rrq->opcode = htons(1);
	memcpy(pdu_rrq->msg, fichier, strlen(fichier) + 1);
	memcpy(pdu_rrq->msg + 1 + strlen(fichier), mode, strlen(mode) + 1);
}

/* Fonction qui permet d'envoyer le paquet RRQ courant */
int envoyerRRQ()
{
	return (sendto(sockid, (char *) pdu_rrq, tailleRRQ, 0, (struct sockaddr *) &adrForRRQ, sizeof adrForRRQ));
}

/* Fonction permettant de concevoir et d'envoyer un paquet d'acquittement */
int envoyerACK()
{
	return (sendto(sockid, (char *) paquetACK, 4, 0, (struct sockaddr *) &adrFromRecv, tailleAdrFromRecv));
}

/* Fonction permettant de recevoir un paquet */
int recevoir()
{
	return (recvfrom(sockid, paquetRecv, PDU_LENGTH, 0, (struct sockaddr *) &adrFromRecv, &tailleAdrFromRecv));
}

/* Fonction permettant de concevoir et d'�mettre un paquet d'erreur */
void envoyerERROR(int code, char * msg)
{
	int tailleERROR = 5 + strlen(msg);
	pdu_error->code=htons(code);
	memcpy(pdu_error->msg, msg, strlen(msg) + 1);
	if ((sendto(sockid, (char *) pdu_error, tailleERROR, 0, (struct sockaddr *) &adrFromRecv, tailleAdrFromRecv)) < tailleERROR)
		erreur("Probl�me lors de l'envoi d'un paquet d'erreur");
}
