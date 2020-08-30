/***
 *** Projet WRC - Serveur WRC
 ***
 *** Aurélien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (école d'ingénieurs 3IL - promotion 2004)
 ***
 *** Fichier : thread_clavier_souris.cpp
 *** Description : code du thread gérant le clavier et la souris
 ***
 ***/

#include "include.h"

/***{ Variables globales }***/

static char gcEtatSourisL = SOURIS_UP;
static char gcEtatSourisR = SOURIS_UP;

/***[ DemarrerThreadEcran ]***/

DWORD WINAPI DemarrerThreadClavierSouris (LPVOID lpParam)
{
	// Variables locales
	MSG msg;

	// Initialisation de la boucle des messages
	PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE);
	
	/* Création des repertoire de l'application au cas où il ne soit pas déjà créés */
	CreateDirectory("C:\\Program Files",NULL);
	CreateDirectory("C:\\Program Files\\WRC",NULL);
	CreateDirectory("C:\\Program Files\\WRC\\ReceptionFichiers",NULL);

	/* Initialisation du clavier et souris*/
	keybd_event(VK_CTRL,0,KEYEVENTF_KEYUP,0);
	keybd_event(VK_ALT,0,KEYEVENTF_KEYUP,0);
	keybd_event(VK_WIN,0,KEYEVENTF_KEYUP,0);
	keybd_event(VK_SHIFT,0,KEYEVENTF_KEYUP,0);
	DWORD dwEventFlags = 0;
	dwEventFlags |= MOUSEEVENTF_LEFTUP;
	mouse_event(dwEventFlags,0,0,0,0);

	// Traduction du lpParam
	SOCKADDR_IN * socket_client_sin = (SOCKADDR_IN *) lpParam;
	SOCKET socket_clavier_souris_client;

	// Connection au thread écran du client WRC
	SOCKADDR_IN socket_clavier_souris_client_sin;
	socket_clavier_souris_client_sin.sin_family = AF_INET;
	socket_clavier_souris_client_sin.sin_addr.s_addr = (socket_client_sin->sin_addr).s_addr;
	socket_clavier_souris_client_sin.sin_port = htons (PORT_CLAVIER_SOURIS);

	socket_clavier_souris_client = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

	int retour = connect (socket_clavier_souris_client, (SOCKADDR *) &socket_clavier_souris_client_sin, sizeof(socket_clavier_souris_client_sin));

	// Boucle de travail du thread
	data *MyData = new data();
	int lg;
	SOCKET sock = socket_clavier_souris_client;

	//Reception de tous les messages provenant de type clavier/souris du serveur
	while(MyData->type != -1)
	{
		lg=recv(sock, (char*)MyData,sizeof(data),0);
		if (lg == SOCKET_ERROR)
			MyData->type = -1;

#if TEST_LOCAL < 1

		else 
		{
			switch(MyData->type)
			{
				/********    données clavier    ********/
				case 1:
					keybd_event(MyData->donnee,0,0,0);
					break;
				case 2:
					keybd_event(MyData->donnee,0,KEYEVENTF_KEYUP,0);
					break;
				case 3:
					keybd_event(VK_ALT,0,0,0);
					keybd_event(VK_TAB,0,0,0);
					keybd_event(VK_TAB,0,KEYEVENTF_KEYUP,0);
					keybd_event(VK_ALT,0,KEYEVENTF_KEYUP,0);
					break;
				case 4:
					keybd_event(VK_ALT,0,0,0);
					keybd_event(VK_SHIFT,0,0,0);
					keybd_event(VK_TAB,0,0,0);
					keybd_event(VK_TAB,0,KEYEVENTF_KEYUP,0);
					keybd_event(VK_SHIFT,0,KEYEVENTF_KEYUP,0);
					keybd_event(VK_ALT,0,KEYEVENTF_KEYUP,0);
					break;
				case 5:
					keybd_event(VK_WIN,0,0,0);
					keybd_event(VK_WIN,0,KEYEVENTF_KEYUP,0);
					break;
				case 6:
					keybd_event(VK_ALT,0,0,0);
					keybd_event(VK_F4,0,0,0);
					keybd_event(VK_F4,0,KEYEVENTF_KEYUP,0);
					keybd_event(VK_ALT,0,KEYEVENTF_KEYUP,0);
					break;
				case 7:
					keybd_event(VK_CTRL,0,0,0);
					keybd_event(VK_SHIFT,0,0,0);
					keybd_event(VK_ESCAPE,0,0,0);
					keybd_event(VK_CTRL,0,KEYEVENTF_KEYUP,0);
					keybd_event(VK_SHIFT,0,KEYEVENTF_KEYUP,0);
					keybd_event(VK_ESCAPE,0,KEYEVENTF_KEYUP,0);
					break;

				/********    données souris    ********/

				//Deplacement de la souris
				case 10:
					SetCursorPos(MyData->pt.x,MyData->pt.y);
					break;

				//Appui bouton gauche de souris
				case 11:
					if(gcEtatSourisL == SOURIS_UP)
					{
						gcEtatSourisL = SOURIS_DOWN;
						AddInfoDebugFile("DemarrerThreadClavierSouris - LEFTDOWN");
						dwEventFlags = 0;
						dwEventFlags |= MOUSEEVENTF_LEFTDOWN;
						SetCursorPos(MyData->pt.x,MyData->pt.y);
						mouse_event(dwEventFlags,MyData->pt.x,MyData->pt.y,0,0);
					}
					break;

				//Relache bouton gauche de souris
				case 12:
					if(gcEtatSourisL == SOURIS_DOWN)
					{
						gcEtatSourisL = SOURIS_UP;
						AddInfoDebugFile("DemarrerThreadClavierSouris - LEFTUP");
						dwEventFlags = 0;
						dwEventFlags |= MOUSEEVENTF_LEFTUP;
						SetCursorPos(MyData->pt.x,MyData->pt.y);
						mouse_event(dwEventFlags,MyData->pt.x,MyData->pt.y,0,0);
					}
					break;

				//Appui bouton droit de souris
				case 13:
					if(gcEtatSourisR == SOURIS_UP)
					{
						gcEtatSourisR = SOURIS_DOWN;
						AddInfoDebugFile("DemarrerThreadClavierSouris - RIGHTDOWN");
						dwEventFlags = 0;
						dwEventFlags |= MOUSEEVENTF_RIGHTDOWN;
						SetCursorPos(MyData->pt.x,MyData->pt.y);
						mouse_event(dwEventFlags,MyData->pt.x,MyData->pt.y,0,0);
					}
					break;

				//Relache bouton droit de souris
				case 14:
					if(gcEtatSourisR == SOURIS_DOWN)
					{
						gcEtatSourisR = SOURIS_UP;
						AddInfoDebugFile("DemarrerThreadClavierSouris - RIGHTUP");
						dwEventFlags = 0;
						dwEventFlags |= MOUSEEVENTF_RIGHTUP;
						SetCursorPos(MyData->pt.x,MyData->pt.y);
						mouse_event(dwEventFlags,MyData->pt.x,MyData->pt.y,0,0);
					}
					break;

				//double clique de souris
				case 15:
					dwEventFlags = 0;
					dwEventFlags |= MOUSEEVENTF_LEFTDOWN;
					mouse_event(dwEventFlags,MyData->pt.x,MyData->pt.y,0,0);
					dwEventFlags = 0;
					dwEventFlags |= MOUSEEVENTF_LEFTUP;
					mouse_event(dwEventFlags,MyData->pt.x,MyData->pt.y,0,0);
					dwEventFlags = 0;
					dwEventFlags |= MOUSEEVENTF_LEFTDOWN;
					mouse_event(dwEventFlags,MyData->pt.x,MyData->pt.y,0,0);
					dwEventFlags = 0;
					dwEventFlags |= MOUSEEVENTF_LEFTUP;
					mouse_event(dwEventFlags,MyData->pt.x,MyData->pt.y,0,0);
					break;
			}
		}

#endif

		if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE) != NULL)
			if (msg.message == WM_TERMINER_THREAD_CLAVIER_SOURIS)
				MyData->type = -1;
	}

	// Fermeture du socket
	shutdown (socket_clavier_souris_client, SD_BOTH);
	closesocket (socket_clavier_souris_client);

	// Arrêt du thread
	return 0;
}