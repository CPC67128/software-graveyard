/***
 *** Projet WRC - Client WRC
 ***
 *** Aurélien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (école d'ingénieurs 3IL - promotion 2004)
 ***
 *** Fichier : thread_fichier.cpp
 *** Description : code du thread gérant le transfere de fichier
 ***
 ***/

#include "include.h"

/***{ Variables globales }***/

extern HANDLE hEventThreadFichier;

/***[ DemarrerThreadFichier ]***/

DWORD WINAPI DemarrerThreadFichier (LPVOID lpParam)
{
	// Ajout d'une information dans le fichier de debug
	AddInfoDebugFile ("DemarrerThreadFichier : début");

	// Variables locales
	SOCKET socket_fichier;
	SOCKADDR_IN socket_fichier_sin;
	SOCKET socket_fichier_client;
	int retour;
	MSG msg;
	bool continuer = true;

	// Déclaration du socket
	socket_fichier_sin.sin_family = AF_INET;
	socket_fichier_sin.sin_addr.s_addr = INADDR_ANY;
	socket_fichier_sin.sin_port = htons(PORT_FICHIER);

	// Création du socket
	AddInfoDebugFile ("DemarrerThreadFichier : socket");

	socket_fichier = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_fichier == SOCKET_ERROR)
	{
		AddErrDebugFile ("DemarrerThreadFichier : socket");
		return ERROR_SOCKET;
	}

	// Enregistrement du socket
	AddInfoDebugFile ("DemarrerThreadFichier : bind");

	int valeur_option = 1;
	retour = setsockopt (socket_fichier, SOL_SOCKET, SO_REUSEADDR, (char *) &valeur_option, sizeof(int));
	if (retour == SOCKET_ERROR)
	{
		AddErrDebugFile ("DemarrerThreadFichier : setsockopt");
		return ERROR_SETSOCKOPT;
	}

	retour = bind (socket_fichier, (SOCKADDR *) &socket_fichier_sin, sizeof(socket_fichier_sin));
	if (retour == SOCKET_ERROR)
	{
		AddErrDebugFile ("DemarrerThreadFichier : bind");
		return ERROR_BIND;
	}

	// Mise en écoute du socket
	AddInfoDebugFile ("DemarrerThreadFichier : listen");

	retour = listen (socket_fichier, 0);
	if (retour == SOCKET_ERROR)
	{
		AddErrDebugFile ("DemarrerThreadFichier : listen");
		return ERROR_LISTEN;
	}

	// Déclenchement de l'évenement de déblocage du thread principal aprés initialisation de la file des messages du thread
	AddInfoDebugFile ("DemarrerThreadFichier : SetEvent");

	PeekMessage (&msg, NULL, 0, 0, PM_REMOVE);

	SetEvent (hEventThreadFichier);

	// Mise en écoute du socket
	AddInfoDebugFile ("DemarrerThreadFichier : accept");

	socket_fichier_client = accept(socket_fichier, NULL, NULL);
	
	fichier *MyFile = new fichier();
	MyFile->FileSize = -1;
	while (continuer && GetMessage(&msg, NULL, 0, 0))
	{
		AddInfoDebugFile ("DemarrerThreadFichier : réception d'un message");
		switch (msg.message)
		{
			case WM_FICHIER :
				AddInfoDebugFile ("DemarrerThreadFichier : WM_FICHIER");
				retour = send (socket_fichier_client, (char *)( msg.wParam), sizeof(fichier), 0);
				if (retour == SOCKET_ERROR)
				{
					continuer = false;
				}
				break;

			case WM_TERMINER_THREAD_FICHIER :
				AddInfoDebugFile ("DemarrerThreadFichier : WM_TERMINER_THREAD_FICHIER");
				continuer = false;
				break;
		}
	}

	// Fermeture du socket
	AddInfoDebugFile ("DemarrerThreadFichier : shutdown");
	shutdown (socket_fichier, SD_BOTH);
	closesocket(socket_fichier);

	// Fin du thread
	AddInfoDebugFile ("DemarrerThreadFichier : fin");
	return TRUE;
}
