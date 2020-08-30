/***
 *** Projet WRC - Client WRC
 ***
 *** Aur�lien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (�cole d'ing�nieurs 3IL - promotion 2004)
 ***
 *** Fichier : thread_clavier_souris.cpp
 *** Description : code du thread g�rant le clavier et la souris
 ***
 ***/

#include "include.h"

/***{ Variables globales }***/

extern HANDLE hEventThreadClavierSouris;

/***[ DemarrerThreadClavierSouris ]***/

DWORD WINAPI DemarrerThreadClavierSouris (LPVOID lpParam)
{
	// Ajout d'une information dans le fichier de debug
	AddInfoDebugFile ("DemarrerThreadClavierSouris : d�but");

	// Variables locales
	SOCKET socket_clavier_souris;
	SOCKADDR_IN socket_clavier_souris_sin;
	SOCKET socket_clavier_souris_client;
	int retour;
	MSG msg;
	bool continuer = true;

	// D�claration du socket
	socket_clavier_souris_sin.sin_family = AF_INET;
	socket_clavier_souris_sin.sin_addr.s_addr = INADDR_ANY;
	socket_clavier_souris_sin.sin_port = htons(PORT_CLAVIER_SOURIS);

	// Cr�ation du socket
	AddInfoDebugFile ("DemarrerThreadClavierSouris : socket");

	socket_clavier_souris = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_clavier_souris == SOCKET_ERROR)
	{
		AddErrDebugFile ("DemarrerThreadClavierSouris : socket");
		return ERROR_SOCKET;
	}

	// Enregistrement du socket
	AddInfoDebugFile ("DemarrerThreadClavierSouris : bind");

	int valeur_option = 1;
	retour = setsockopt (socket_clavier_souris, SOL_SOCKET, SO_REUSEADDR, (char *) &valeur_option, sizeof(int));
	if (retour == SOCKET_ERROR)
	{
		AddErrDebugFile ("DemarrerThreadClavierSouris : setsockopt");
		return ERROR_SETSOCKOPT;
	}

	retour = bind (socket_clavier_souris, (SOCKADDR *) &socket_clavier_souris_sin, sizeof(socket_clavier_souris_sin));
	if (retour == SOCKET_ERROR)
	{
		AddErrDebugFile ("DemarrerThreadClavierSouris : bind");
		return ERROR_BIND;
	}

	// Mise en �coute du socket
	AddInfoDebugFile ("DemarrerThreadClavierSouris : listen");

	retour = listen (socket_clavier_souris, 0);
	if (retour == SOCKET_ERROR)
	{
		AddErrDebugFile ("DemarrerThreadClavierSouris : listen");
		return ERROR_LISTEN;
	}

	// D�clenchement de l'�venement de d�blocage du thread principal apr�s initialisation de la file des messages du thread
	AddInfoDebugFile ("DemarrerThreadClavierSouris : SetEvent");

	PeekMessage (&msg, NULL, 0, 0, PM_REMOVE);

	SetEvent (hEventThreadClavierSouris);

	// Mise en �coute du socket
	AddInfoDebugFile ("DemarrerThreadClavierSouris : accept");

	socket_clavier_souris_client = accept(socket_clavier_souris, NULL, NULL);

	data * mydata = new data();
	mydata->type = -1;
	while (continuer && GetMessage(&msg, NULL, 0, 0))
	{
		AddInfoDebugFile ("DemarrerThreadClavierSouris : r�ception d'un message");
		switch (msg.message)
		{
			case WM_CLAVIER :
				AddInfoDebugFile ("DemarrerThreadClavierSouris : WM_CLAVIER");
				retour = send (socket_clavier_souris_client, (char *) msg.wParam, sizeof(data), 0);
				if (retour == SOCKET_ERROR)
				{
					continuer = false;
				}
				break;

			case WM_SOURIS :
				AddInfoDebugFile ("DemarrerThreadClavierSouris : WM_SOURIS");
				retour = send (socket_clavier_souris_client, (char *) msg.wParam, sizeof(data), 0);
				if (retour == SOCKET_ERROR)
				{
					continuer = false;
				}
				break;

			case WM_TERMINER_THREAD_CLAVIER_SOURIS :
				AddInfoDebugFile ("DemarrerThreadClavierSouris : WM_TERMINER_THREAD_CLAVIER_SOURIS");
				continuer = false;
				break;
		}
	}

	// Fermeture du socket
	AddInfoDebugFile ("DemarrerThreadClavierSouris : shutdown");
	shutdown (socket_clavier_souris, SD_BOTH);
	closesocket(socket_clavier_souris);

	// Fin du thread
	AddInfoDebugFile ("DemarrerThreadClavierSouris : fin");
	return TRUE;
}
