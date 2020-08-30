/***
 *** Projet WRC - Client WRC
 ***
 *** Aur�lien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (�cole d'ing�nieurs 3IL - promotion 2004)
 ***
 *** Fichier : thread_ecran.cpp
 *** Description : code du thread g�rant l'�cran
 ***
 ***/

#include "include.h"

/***{ Variables globales }***/

extern HANDLE hEventThreadEcran;
Ecran informations;

/***[ DemarrerThreadEcran ]***/

DWORD WINAPI DemarrerThreadEcran (LPVOID lpParam)
{
	// Ajout d'une information dans le fichier de debug
	AddInfoDebugFile ("DemarrerThreadEcran : d�but");

	// Variables locales
	SOCKET socket_ecran;
	SOCKADDR_IN socket_ecran_sin;
	SOCKET socket_ecran_client;
	int retour;
	ParamThreadEcran pte;

	// Traduction du lpParam
	pte.hWnd = ((ParamThreadEcran *) lpParam)->hWnd;
	pte.resolution = ((ParamThreadEcran *) lpParam)->resolution;
	pte.pThreadClavierSourisId = ((ParamThreadEcran *) lpParam)->pThreadClavierSourisId;

	// D�claration du socket
	socket_ecran_sin.sin_family = AF_INET;
	socket_ecran_sin.sin_addr.s_addr = INADDR_ANY;
	socket_ecran_sin.sin_port = htons(PORT_ECRAN);

	// Cr�ation du socket
	AddInfoDebugFile ("DemarrerThreadEcran : socket");

	socket_ecran = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_ecran == SOCKET_ERROR)
	{
		AddErrDebugFile ("DemarrerThreadEcran : socket");
		return ERROR_SOCKET;
	}

	// Enregistrement du socket
	AddInfoDebugFile ("DemarrerThreadEcran : bind");

	int valeur_option = 1;
	retour = setsockopt (socket_ecran, SOL_SOCKET, SO_REUSEADDR, (char *) &valeur_option, sizeof(int));
	if (retour == SOCKET_ERROR)
	{
		AddErrDebugFile ("DemarrerThreadEcran : setsockopt");
		return ERROR_SETSOCKOPT;
	}

	retour = bind (socket_ecran, (SOCKADDR *) &socket_ecran_sin, sizeof(socket_ecran_sin));
	if (retour == SOCKET_ERROR)
	{
		AddErrDebugFile ("DemarrerThreadEcran : bind");
		return ERROR_BIND;
	}

	// Mise en �coute du socket
	AddInfoDebugFile ("DemarrerThreadEcran : listen");

	retour = listen (socket_ecran, 0);
	if (retour == SOCKET_ERROR)
	{
		AddErrDebugFile ("DemarrerThreadEcran : listen");
		return ERROR_LISTEN;
	}

	// D�clenchement de l'�venement de d�blocage du thread principal
	AddInfoDebugFile ("DemarrerThreadEcran : SetEvent");

	SetEvent (hEventThreadEcran);

	// Mise en �coute du socket
	AddInfoDebugFile ("DemarrerThreadEcran : accept");

	socket_ecran_client = accept(socket_ecran, NULL, NULL);

	informations.hwnd=pte.hWnd;
	informations.Bmp.height=0;
	informations.Bmp.width =0;
	informations.Bmp.PixelSize =0;
	informations.Bmp.Numcolors =0;
	informations.Continuer=1;

	// Boucle de r�ception des images
	BoucleReceptionImage((Ecran *) &informations, socket_ecran_client);

	// Fermeture du socket
	AddInfoDebugFile ("DemarrerThreadEcran : shutdown");
	shutdown (socket_ecran, SD_BOTH);
	closesocket(socket_ecran);

	// Fin du thread
	AddInfoDebugFile ("DemarrerThreadEcran : fin");
	return TRUE;
}
