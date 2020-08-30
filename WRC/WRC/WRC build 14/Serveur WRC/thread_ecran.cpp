/***
 *** Projet WRC - Serveur WRC
 ***
 *** Aurélien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (école d'ingénieurs 3IL - promotion 2004)
 ***
 *** Fichier : thread_ecran.cpp
 *** Description : code du thread gérant l'écran
 ***
 ***/

#include "include.h"

/***{ Variables globales }***/

bool initialiser;

/***[ DemarrerThreadEcran ]***/

DWORD WINAPI DemarrerThreadEcran (LPVOID lpParam)
{
	// Ajout d'une information dans le fichier de debug
	AddInfoDebugFile ("DemarrerThreadEcran : début");

	// Variables locales
	int retour;
	SOCKADDR_IN * socket_client_sin;
	SOCKADDR_IN socket_ecran_client_sin;
	SOCKET socket_ecran_client;
	MSG msg;

	// Intialisation de la file des messages
	PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE);

	// Traduction du lpParam
	socket_client_sin = (SOCKADDR_IN *) lpParam;

	// Création du socket du thread
	AddInfoDebugFile ("DemarrerThreadEcran : socket");

	socket_ecran_client = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_ecran_client == SOCKET_ERROR)
	{
		AddErrDebugFile ("DemarrerThreadEcran : socket");
		return ERROR_SOCKET;
	}

	// Connection au thread écran du client WRC
	AddInfoDebugFile ("DemarrerThreadEcran : connect");

	socket_ecran_client_sin.sin_family = AF_INET;
	socket_ecran_client_sin.sin_addr.s_addr = (socket_client_sin->sin_addr).s_addr;
	socket_ecran_client_sin.sin_port = htons (PORT_ECRAN);

	retour = connect (socket_ecran_client, (SOCKADDR *) &socket_ecran_client_sin, sizeof (socket_ecran_client_sin));
	if (retour == SOCKET_ERROR)
	{
		AddErrDebugFile ("DemarrerThreadEcran : connect");
		return ERROR_CONNECT;
	}

	// Appel des fonctions d'Aurélien
	BoucleEnvoiImage (socket_ecran_client);

	// Fermeture du socket
	AddInfoDebugFile ("DemarrerThreadEcran : shutdown");
	shutdown (socket_ecran_client, SD_BOTH);

	AddInfoDebugFile ("DemarrerThreadEcran : closesocket");
	closesocket (socket_ecran_client);

	// Arrêt du thread
	AddInfoDebugFile ("DemarrerThreadEcran : fin");
	return 0;
}
