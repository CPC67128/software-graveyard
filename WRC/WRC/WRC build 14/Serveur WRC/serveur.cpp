/***
 *** Projet WRC - Serveur WRC
 ***
 *** Aurélien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (école d'ingénieurs 3IL - promotion 2004)
 ***
 *** Fichier : serveur.cpp
 *** Description : programme principal du serveur WRC
 ***
 ***/

#include "include.h"

/***{ Variables globales }***/

SOCKET socket_serveur;
SOCKET socket_client = NULL;
SOCKADDR_IN socket_client_sin;

HMENU hMenuPopup;

HANDLE hThread[3];

char * password;

INFOSERVEUR infoServeur;
INFOCLIENT infoClient;

DWORD ThreadIdEcran;
DWORD ThreadIdClavierSouris;
DWORD ThreadIdFichier;

/***[ WinMain ]***/

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	HANDLE hMutex = CreateMutex(NULL, FALSE, "MutexServerWRC");
	if(!hMutex)
	{ 
		MessageBox (NULL, TEXT("Erreur à la création du mutex"), TEXT("Serveur WRC"), MB_OK | MB_ICONERROR);
		exit(0);
	} 

	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox (NULL, TEXT("Le serveur est déjà en fonction !"), TEXT("Serveur WRC"), MB_OK | MB_ICONWARNING);
		CloseHandle(hMutex);
		exit(0);
	}

	// Initialisation du fichier de debug
	InitDebugFile ();
	AddInfoDebugFile ("Démarrage de l'application Serveur WRC");

	// Initialisation du générateur de nombres aléatoires
	srand ((unsigned) time (NULL));

	// Initialisation des WSA
	WSADATA wsadata;
	WSAStartup (MAKEWORD (2, 2), &wsadata);

	// Récupération de la résolution d'écran
	infoServeur.largeurEcran = GetSystemMetrics(SM_CXSCREEN);
	infoServeur.hauteurEcran = GetSystemMetrics(SM_CYSCREEN);

	// Récupération du mot de passe
	password = (char *) malloc (TAILLE_BUFFER);
	if (LirePassword (&password, TAILLE_BUFFER))
		infoServeur.password = TRUE;
	else
		infoServeur.password = FALSE;

	// Création de la boîte de dialogue du serveur
	HWND hWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_SERVEUR_WRC), NULL, ServerMainDialogProc);

	// Initialisation de l'icône systray
	InitSysTrayIcon (hWnd, (HICON) LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_WRC_SERVER_HALTED), IMAGE_ICON, 16, 16, 0), "Serveur WRC");
	hMenuPopup = GetSubMenu(LoadMenu ((HINSTANCE) GetModuleHandle (NULL), MAKEINTRESOURCE (IDR_MENU)), 0);
	AddIcon ();

	// Démarrage du serveur
	socket_serveur = CreateServerSocket (hWnd);
	EnableMenuItem (hMenuPopup, ID_MENU_DEMARRER, MF_GRAYED);
	EnableMenuItem (hMenuPopup, ID_MENU_ARRETER, MF_ENABLED);
	ChangeIcon ((HICON) LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_WRC_DISCONNECTED), IMAGE_ICON, 16, 16, 0));

	// Boucle des messages
	MSG msg;
	while (GetMessage (&msg, NULL, 0, 0))
	{
		if (hWnd || !IsDialogMessage (hWnd, &msg))
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}
	}

	// Fin de l'application
	RemoveIcon ();
	WSACleanup ();

    // Ferme le mutex
    CloseHandle(hMutex);
	AddInfoDebugFile ("Fin de l'application Serveur WRC");
	CloseDebugFile ();
	return 0;
}

/***[ RejectConnection ]***/

int CALLBACK RejectConnection (IN LPWSABUF lpCallerId, IN LPWSABUF lpCallerData, IN OUT LPQOS lpSQOS, IN OUT LPQOS lpGQOS, IN LPWSABUF lpCalleeId, OUT LPWSABUF lpCalleeData, OUT GROUP FAR * g, IN DWORD dwCallbackData)
{
	return CF_REJECT;
}

/***[ ServerMainDialogProc ]***/

BOOL CALLBACK ServerMainDialogProc (HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	// Variables locales
	char buffer_reception [TAILLE_BUFFER];
	char buffer_emission [TAILLE_BUFFER];
	char * szPassword;
	static char * szWallpaper;
	DWORD choixPopupMenu;
	int retour;
	int socket_ecran_serveur_sin_length;
	unsigned __int64 i64Password;
	bool correspondent;
	SOCKET socket_client_trash = NULL;
	static int ad;

	// Traitement du message
	switch (Msg)
	{
		case WM_INITDIALOG :
			if (infoServeur.password)
			{
				SendMessage (GetDlgItem (hWnd, IDC_PASSWORD), BM_SETCHECK, (WPARAM) MF_CHECKED, (LPARAM) NULL);
				EnableWindow (GetDlgItem (hWnd, IDC_EDIT_PASSWORD), true);
			}
			else
			{
				SendMessage (GetDlgItem (hWnd, IDC_PASSWORD), BM_SETCHECK, (WPARAM) MF_UNCHECKED, (LPARAM) NULL);
				EnableWindow (GetDlgItem (hWnd, IDC_EDIT_PASSWORD), false);
			}
			return TRUE;

		case WM_CLOSE :
			ShowWindow(hWnd, SW_HIDE);
			return TRUE;

		case WM_COMMAND :
			switch (LOWORD (wParam))
			{
				case IDC_PASSWORD :
					if (SendMessage (GetDlgItem (hWnd, IDC_PASSWORD), BM_GETCHECK, NULL, NULL) == BST_CHECKED)
						EnableWindow (GetDlgItem (hWnd, IDC_EDIT_PASSWORD), true);
 					else
						EnableWindow (GetDlgItem (hWnd, IDC_EDIT_PASSWORD), false);
					return TRUE;

				case IDC_APPLIQUER :
					if (SendMessage (GetDlgItem (hWnd, IDC_PASSWORD), BM_GETCHECK, NULL, NULL) == BST_CHECKED)
					{
						SendMessage (GetDlgItem (hWnd, IDC_EDIT_PASSWORD), WM_GETTEXT, (WPARAM) TAILLE_BUFFER, (LPARAM) &buffer_reception);
						if (strlen (buffer_reception) == 0)
							MessageBox (hWnd, TEXT("Vous devez spécifier un mot de passe !"), TEXT("Serveur WRC"), MB_OK | MB_ICONWARNING);
						else
						{
							szPassword = algorithme_sha (buffer_reception);
							EnregistrerPassword (szPassword);
							LirePassword (&password, TAILLE_BUFFER);
							infoServeur.password = TRUE;
						}
					}
 					else
					{
						SupprimerPassword ();
						infoServeur.password = FALSE;
					}

					SendMessage (GetDlgItem (hWnd, IDC_EDIT_PASSWORD), WM_SETTEXT, (WPARAM) NULL, (LPARAM) TEXT(""));
					ShowWindow(hWnd, SW_HIDE);
					return TRUE;

				case IDC_ANNULER :
					SendMessage (GetDlgItem (hWnd, IDC_EDIT_PASSWORD), WM_SETTEXT, (WPARAM) NULL, (LPARAM) TEXT(""));
					if (infoServeur.password)
					{
						SendMessage (GetDlgItem (hWnd, IDC_PASSWORD), BM_SETCHECK, (WPARAM) MF_CHECKED, (LPARAM) NULL);
						EnableWindow (GetDlgItem (hWnd, IDC_EDIT_PASSWORD), true);
					}
					else
					{
						SendMessage (GetDlgItem (hWnd, IDC_PASSWORD), BM_SETCHECK, (WPARAM) MF_UNCHECKED, (LPARAM) NULL);
						EnableWindow (GetDlgItem (hWnd, IDC_EDIT_PASSWORD), false);
					}
					ShowWindow(hWnd, SW_HIDE);
					return TRUE;
			}
			break;

		case WM_SOCKET_NOTIFY :
			AddInfoDebugFile ("ServerMainDialogProc : WM_SOCKET_NOTIFY");

			switch (WSAGETSELECTEVENT (lParam))
			{
				case FD_ACCEPT:
					AddInfoDebugFile ("ServerMainDialogProc : FD_ACCEPT");

					if (WSAGETSELECTERROR (lParam))
					{
						AddErrDebugFile ("ServerMainDialogProc : FD_ACCEPT");
						return TRUE;
					}

					if (socket_client == NULL)
					{
						AddInfoDebugFile ("ServerMainDialogProc : FD_ACCEPT : ACCEPT");
						socket_ecran_serveur_sin_length = sizeof (socket_client_sin);
						socket_client = accept(socket_serveur, (SOCKADDR *) &socket_client_sin, &socket_ecran_serveur_sin_length);
						ChangeIcon ((HICON) LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_WRC_CONNECTED), IMAGE_ICON, 16, 16, 0));
					}
					else
					{
						socket_client_trash = WSAAccept(socket_serveur, NULL, NULL, RejectConnection, NULL);
						closesocket(socket_client_trash);
						return FALSE;
					}
					break;

				case FD_CLOSE :
					AddInfoDebugFile ("ServerMainDialogProc : FD_CLOSE");
					shutdown (socket_client, 2);
					closesocket (socket_client);
					socket_client = NULL;
					PostThreadMessage (ThreadIdEcran, WM_TERMINER_THREAD_ECRAN, NULL, NULL);
					PostThreadMessage (ThreadIdClavierSouris, WM_TERMINER_THREAD_CLAVIER_SOURIS, NULL, NULL);
					TerminateThread(hThread[2],0);
					WaitForMultipleObjects (3, hThread, TRUE, INFINITE);
					ChangeIcon ((HICON) LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_WRC_DISCONNECTED), IMAGE_ICON, 16, 16, 0));
					MettreArrierePlan (szWallpaper, ad);
					break;

				case FD_READ:
					AddInfoDebugFile ("ServerMainDialogProc : FD_READ");

					// Réception des informations
					retour = recv (socket_client, buffer_reception, TAILLE_BUFFER, 0);
					if (retour == SOCKET_ERROR)
					{
						AddErrDebugFile ("ServerMainDialogProc : FD_READ : recv");
						return TRUE;
					}

					if (strncmp (buffer_reception, CODE_CONNECTION_REQUESTED, strlen(CODE_CONNECTION_REQUESTED)) == 0)
					{
						sscanf (buffer_reception + strlen(CODE_CONNECTION_REQUESTED) + 3, "%d %d %d %d %d %d", &(infoClient.methodeEcran), &(infoClient.methodeEcranOptions), &(infoClient.sleepLength), &(infoClient.modeTransmission), &(infoClient.modeTransmissionOptions), &(infoClient.limitToSystray));

						if (infoServeur.password)
						{
							CompleterRSAInfoServeur (&infoServeur);
							RenseignerSomme (password, strlen(password));
						}

						infoServeur.largeurEcran = GetSystemMetrics(SM_CXSCREEN);
						infoServeur.hauteurEcran = GetSystemMetrics(SM_CYSCREEN);
						sprintf(buffer_emission, "INFOSERVEUR : %d %d %d %I64u %I64u", infoServeur.largeurEcran, infoServeur.hauteurEcran, infoServeur.password, infoServeur.n, infoServeur.e);
						retour = send (socket_client, buffer_emission, strlen (buffer_emission) + 1, 0);
					}
					if (strncmp (buffer_reception, "PASSWORD : ", strlen("PASSWORD : ")) == 0)
					{
						sscanf (buffer_reception, "PASSWORD : %I64u", &i64Password);
						
						sprintf (buffer_reception, "PASSWORD %I64u", i64Password);
						AddInfoDebugFile (buffer_reception);

						correspondent = ComparerPassword (i64Password);
						if (correspondent)
							retour = send (socket_client, CODE_CONNECTION_ACCEPTED, strlen(CODE_CONNECTION_ACCEPTED) + 1, 0);
						else
							retour = send (socket_client, CODE_CONNECTION_REFUSED, strlen(CODE_CONNECTION_REFUSED) + 1, 0);
					}
					if (strcmp (buffer_reception, CODE_INFOSERVEUR_NOPASSWORD_RECEIVED) == 0)
					{
						retour = send (socket_client, CODE_CONNECTION_ACCEPTED, strlen(CODE_CONNECTION_ACCEPTED) + 1, 0);
					}
					if (strcmp (buffer_reception, CODE_CONNECTION_CONFIRMED) == 0)
					{
						szWallpaper = SupprimerArrierePlan (&ad);
						hThread [0] = CreateThread(NULL, NULL, DemarrerThreadEcran, (LPVOID) &socket_client_sin, NULL, &ThreadIdEcran);
						hThread [1] = CreateThread(NULL, NULL, DemarrerThreadClavierSouris, (LPVOID) &socket_client_sin, NULL, &ThreadIdClavierSouris);
						hThread [2] = CreateThread(NULL, NULL, DemarrerThreadFichier, (LPVOID) &socket_client_sin, NULL, &ThreadIdFichier);
					}
					break;
			}
			return 0;

		case MYWM_NOTIFYICON :
			switch (lParam)
			{
				case WM_LBUTTONDBLCLK :
					if (wParam == SYSTRAY_UID)
					{
						if (IsWindowVisible (hWnd))
							ShowWindow(hWnd, SW_HIDE);
						else
							ShowWindow(hWnd, SW_NORMAL);
					}
					return TRUE;

				case WM_RBUTTONUP :
					if (wParam == SYSTRAY_UID)
					{
						choixPopupMenu = ShowMenu (hMenuPopup);
						switch (choixPopupMenu)
						{
							case ID_MENU_DEMARRER :
								socket_serveur = CreateServerSocket (hWnd);
								EnableMenuItem (hMenuPopup, ID_MENU_DEMARRER, MF_GRAYED);
								EnableMenuItem (hMenuPopup, ID_MENU_ARRETER, MF_ENABLED);
								ChangeIcon ((HICON) LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_WRC_DISCONNECTED), IMAGE_ICON, 16, 16, 0));
								break;

							case ID_MENU_ARRETER :
								CloseServerSocket (socket_serveur);
								EnableMenuItem (hMenuPopup, ID_MENU_DEMARRER, MF_ENABLED);
								EnableMenuItem (hMenuPopup, ID_MENU_ARRETER, MF_GRAYED);
								ChangeIcon ((HICON) LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_WRC_SERVER_HALTED), IMAGE_ICON, 16, 16, 0));
								break;

							case ID_MENU_QUITTER :
								DestroyWindow (hWnd);
								PostQuitMessage (0);
								break;

							case ID_MENU_SERVEUR_WRC :
								if (IsWindowVisible (hWnd))
									ShowWindow(hWnd, SW_HIDE);
								else
									ShowWindow(hWnd, SW_NORMAL);
								break;
						}
					}
					return TRUE;
			}
			return TRUE;
	}
	return false;
}
