/*********************************
 *  Client HTTP  *  FUCHS Steve  *
 *********************************/


/***** Inclusions des différentes librairies C *****/
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>


/* Déclarations des constantes */
#define REQUEST_MAX_LENGTH 1500


/***** Prototypes des fonctions utilisées *****/
void erreur(char * s);


/***** Déclarations des variables globales *****/
int fin_boucle = 1;


/***** Programme éxécutable *****/
main (int argc, char ** argv)
{
	/* Déclarations des variables */
	u_short port;
	int fd;
	int sockid, ns, tailleGuest, nbCarLus, retSelect;
	struct sockaddr_in host;
	struct sockaddr guest;
	char requete [REQUEST_MAX_LENGTH];
	int requeteIndice;
	char caractere[1];
	unsigned short nb_spaces;
	char fichier [64];
	char version [64];
	char buffer[1024];
	char fichierAOuvrir [70];
	pid_t pid = 1;

	/* Vérification de la viabilité des arguments */
	if (argc > 2)
		erreur("Syntaxe : myhttp [<port>]");

	/* Récupération du numéro de port */
	if (argc - 2)
		port = getuid() + 10000;
	else
		port = atoi(argv[1]);

	/* Affichage du bon démarrage de l'application */
	printf("-== SERVEUR MYHTTP ==-\n");
	printf("port utilisé : %d\n", port);

	/* Création du socket */
	if ((sockid = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		erreur("Création du socket de communication");

	/* Attachement du socket */
	host.sin_family = AF_INET;
	host.sin_addr.s_addr = htonl(INADDR_ANY);
	host.sin_port = htons(port);
	if (bind(sockid, (struct sockaddr *) &host, sizeof host) < 0)
		erreur("Attachement d'une adresse à un socket");

	/* Déclaration du socket comme socket de connexion */
	if (listen(sockid, 2) < 0)
		erreur("Déclaration du socket comme socket de connexion");

	/* Boucle de service */
	signal(SIGCHLD, SIG_IGN);
	while (fin_boucle)
	{
		/* Attente d'une demande de connexion */
		tailleGuest = sizeof guest;
		if ((ns = accept(sockid, &guest, &tailleGuest)) < 0)
			erreur("Attente d'une demande de connexion");

		pid = fork();
		if (pid == 0 || pid < 0)
		{
			if (pid == 0) fin_boucle = 0;

			/* Lecture de la requête */
			read(ns, requete, REQUEST_MAX_LENGTH);

			/* Extraction du nom du fichier */
			if (sscanf(requete, "GET %s %s", fichier, version) == 2)
			{
				sprintf(fichierAOuvrir, "www%s", fichier);

				/* Ouverture du fichier et envoi de celui-ci s'il existe */
				if ((fd = open(fichierAOuvrir, O_RDONLY)) < 0)
				{
					if (errno == ENOENT)
					{
						if ((fd = open("www/erreur404.html", O_RDONLY)) < 0)
							erreur("Ouverture d'un fichier");
						else
						{
							while ((nbCarLus = read(fd, buffer, 1024)) > 0)
								write(ns, buffer, nbCarLus);
							close(fd);
						}
					}
					else
						erreur("Ouverture d'un fichier");
				}
				else
				{
					while ((nbCarLus = read(fd, buffer, 1024)) > 0)
						write(ns, buffer, nbCarLus);
					close(fd);
				}
			}
		}

		/* Fermeture de la communication coté serveur */
		close(ns);
	}

	/* Fin du serveur */
	if (pid > 0) printf("-== FIN DU SERVEUR ==-\n");
	exit(0);
}


/***** Fonctions du programme *****/

/* Procédure erreur du programme */
void erreur(char * s)
{
	if (errno)	perror(s);
	else	fprintf(stderr, "%s\n", s);
	exit(1);
}
