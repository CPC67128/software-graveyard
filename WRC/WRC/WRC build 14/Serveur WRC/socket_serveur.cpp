/***
 *** Projet WRC - Serveur WRC
 ***
 *** Aurélien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (école d'ingénieurs 3IL - promotion 2004)
 ***
 *** Fichier : socket_serveur.cpp
 *** Description : fonctions de gestion du socket principal du serveur
 ***
 ***/

#include "include.h"

/***[ CreateServerSocket ]***/

SOCKET CreateServerSocket (HWND hWnd)
{
	// Variables locales
	int retour;
	SOCKET socket_serveur;
	SOCKADDR_IN socket_serveur_sin;

	// Déclaration du socket
	socket_serveur_sin.sin_family = AF_INET;
	socket_serveur_sin.sin_addr.s_addr = INADDR_ANY;
	socket_serveur_sin.sin_port = htons(PORT_SERVEUR);

	// Création du socket
	AddInfoDebugFile ("CreateServerSocket : socket");
	socket_serveur = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Demande de gestion par messages pour les évenements réseaux du socket
	AddInfoDebugFile ("CreateServerSocket : WSAAsyncSelect");
	retour = WSAAsyncSelect (socket_serveur, hWnd, WM_SOCKET_NOTIFY, FD_ACCEPT | FD_READ | FD_CLOSE);
	if (retour == SOCKET_ERROR)
	{
		AddErrDebugFile ("CreateServerSocket : WSAAsyncSelect");
		return NULL;
	}

	// Enregistrement du socket
	AddInfoDebugFile ("CreateServerSocket : bind");
	int valeur_option = 1;
	retour = setsockopt(socket_serveur, SOL_SOCKET, SO_REUSEADDR, (char *) &valeur_option, sizeof(int));
	retour = bind (socket_serveur, (SOCKADDR *) &socket_serveur_sin, sizeof(socket_serveur_sin));
	if (retour == SOCKET_ERROR)
	{
		AddErrDebugFile ("CreateServerSocket : bind");
		return NULL;
	}

	// Mise en écoute du socket
	AddInfoDebugFile ("CreateServerSocket : listen");
	retour = listen(socket_serveur, 0);
	if (retour == SOCKET_ERROR)
	{
		AddErrDebugFile ("CreateServerSocket : listen");
		return NULL;
	}

	return socket_serveur;
}

/***[ DestroyServerSocket ]***/

BOOL CloseServerSocket (SOCKET socket_serveur)
{
	shutdown (socket_serveur, 2);
	closesocket(socket_serveur);

	return TRUE;
}
