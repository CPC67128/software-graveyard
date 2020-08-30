/***
 *** Projet WRC - Serveur WRC
 ***
 *** Aurélien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (école d'ingénieurs 3IL - promotion 2004)
 ***
 *** Fichier : thread_fichier.cpp
 *** Description : code du thread gérant le transfert de fichier
 ***
 ***/

#include "include.h"

const char * PATH = "C:\\Program Files\\WRC\\ReceptionFichiers\\";
char *Buffer = (char*)malloc(sizeof(fichier));  
fichier *MyFile = (fichier*)malloc(sizeof(fichier));
char NomFichier[128];
int ALiberer = 0;

DWORD WINAPI DemarrerThreadFichier (LPVOID lpParam)
{
	ALiberer = 1;
	// Variables locales
	MSG msg;
	int lg;
	FILE *pfic;
	// Initialisation de la boucle des messages
	PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE);

	// Traduction du lpParam
	SOCKADDR_IN * socket_client_sin = (SOCKADDR_IN *) lpParam;
	SOCKET socket_fichier_client;

	// Connection au thread écran du client WRC
	SOCKADDR_IN socket_fichier_client_sin;
	socket_fichier_client_sin.sin_family = AF_INET;
	socket_fichier_client_sin.sin_addr.s_addr = (socket_client_sin->sin_addr).s_addr;
	socket_fichier_client_sin.sin_port = htons (PORT_FICHIER);

	socket_fichier_client = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

	int retour = connect (socket_fichier_client, (SOCKADDR *) &socket_fichier_client_sin, sizeof(socket_fichier_client_sin));

	unsigned long positionFichier =0;
	// Boucle de travail du thread
	SOCKET sock = socket_fichier_client;

	//Reception de tous les messages provenant de type fichiers du serveur
	
	int iOuvert = 0;
	int iFini = 0;
	while(iFini != 1)
	{
		lg=recv(sock, (char*)MyFile,sizeof(fichier),0);
		if (lg == SOCKET_ERROR)
			MyFile->FileSize = -1;
		else
		{
			//Ouverture du fichier à créer
			if(iOuvert == 0)
			{
				strcpy(NomFichier,PATH);
				strcat(NomFichier,MyFile->FileName);;
				pfic = fopen(NomFichier ,"wb+");
				iOuvert = 1;
			}

			//Ecriture dans le fichier
			if(iOuvert == 1)
				fwrite(MyFile->DataFile,sizeof(char),MyFile->DataSendSize, pfic);
			
			//Si c'est le dernier paquet envoyé je ferme le fichier 
			if(MyFile->Finish == 1)
			{
				fclose(pfic);
				iOuvert = 0;
				MessageBox(NULL,"Copie terminée avec succès","Info",MB_OK);
			}
			
		}	  
	}
	if(iOuvert == 1)
	{
		fclose(pfic);
		iOuvert = 0;
	}
	MessageBox(NULL,"FinCopie","Info",NULL);
	// Fermeture du socket
	shutdown (socket_fichier_client, SD_BOTH);
	closesocket (socket_fichier_client);

	// Arrêt du thread
	return 0;
}
