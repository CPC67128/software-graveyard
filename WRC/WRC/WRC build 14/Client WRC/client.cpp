/***
 *** Projet WRC - Client WRC
 ***
 *** Aurélien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (école d'ingénieurs 3IL - promotion 2004)
 ***
 *** Fichier : client.cpp
 *** Description : programme principal du client WRC
 ***
 ***/

#include "include.h"

/***{ Variables globales }***/

INFOSERVEUR infoServeur;
INFOCLIENT infoClient;

HANDLE hThread [3];
HANDLE hEventThreadEcran;
HANDLE hEventThreadClavierSouris;
HANDLE hEventThreadFichier;

HDC hdcScreen;
HBITMAP hdcScreenBmp;

HMENU hMenu;

RECT dimensionsFenetre;

SOCKET socket_client;

SOCKADDR_IN socket_serveur_sin;

DWORD ThreadClavierSourisId;
DWORD ThreadEcranId;
DWORD ThreadFichierId;

char * ip_serveur;
char * password;

int hauteur_fenetre, largeur_fenetre;
int hscroll_max_range;
int vscroll_max_range;

extern unsigned __int64 n;
extern unsigned __int64 e;

bool quitter;
bool pleinEcran;
bool afficherScrollBars;
bool afficherMenu = false;
bool refreshMouse = true;

HINSTANCE ghInstance;

CRITICAL_SECTION sectionCritiqueEcran;

static char gcEtatSourisL;
static char gcEtatSourisR;

/***[ WinMain ]***/

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	// Ouverture du fichier de debug et initialisation
	InitDebugFile ();
	AddInfoDebugFile ("Démarrage de l'application Client WRC");

	// Initialisation des WSA
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2,0), &wsadata);

	// Initialisation de la section critique
	InitializeCriticalSection (&sectionCritiqueEcran);

	// Initialisation des variables globales
	ghInstance = hInstance;
	quitter = false;
	ip_serveur = (char *) malloc (TAILLE_BUFFER);
	pleinEcran = false;
	afficherScrollBars = true;
	gcEtatSourisL = SOURIS_UP;
	gcEtatSourisR = SOURIS_UP;

	// Déclarations des variables locales
	bool adresseIPValide = false;
	int retour;
	static TCHAR szClassName [] = TEXT("Client WRC");
	HWND hWnd;
	MSG msg;
	WNDCLASS wndclass;

	// Lecture de l'adresse IP de la dernière connection
	LireInfoClient (&infoClient);
	if (!(LireDerniereAdresseIP (&ip_serveur, TAILLE_BUFFER)))
	{
		ip_serveur [0] = '\0';
	}

	// Demande de l'adresse IP du serveur
	while (!adresseIPValide)
	{
		// Affichage de la boîte de dialogue d'ouverture de session
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_OUVERTURE_DE_SESSION), NULL, ConnexionDialogProc);

		// Vérifions que l'utilisateur n'a pas choisi de quitter l'application
		if (quitter)
		{
			free (ip_serveur);
			WSACleanup();
			return 0;
		}

		// Vérification de l'adresse IP
		adresseIPValide = ResolutionAdresseIP ();
	}

	// Enregistrement de l'adresse IP
	EnregistrerInfoClient (&infoClient);
	EnregistrerDerniereAdresseIP (ip_serveur);

	// Création de la classe de fenêtre
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = ClientWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_WRC), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject(LTGRAY_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szClassName;

	// Enregistrement de la classe de fenêtre
	if(!RegisterClass(&wndclass))
	{
		AddErrDebugFile ("WinMain : RegisterClass");
		free (ip_serveur);
		WSACleanup();
		return 0;
	}

	// Création du menu
	hMenu = LoadMenu (NULL, MAKEINTRESOURCE (IDR_MENU));

	// Création de la fenêtre
	hWnd = CreateWindowEx (WS_EX_OVERLAPPEDWINDOW, szClassName, TEXT("Client WRC"), WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, hMenu, hInstance, NULL);

	// Création du socket
	AddInfoDebugFile ("WinMain : socket");
	socket_client = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Demande de gestion par messages pour les évenements réseaux du socket
	AddInfoDebugFile ("WinMain : WSAAsyncSelect");
	retour = WSAAsyncSelect(socket_client, hWnd, WM_SOCKET_NOTIFY, FD_CONNECT | FD_READ | FD_CLOSE);
	if (retour == SOCKET_ERROR)
	{
		AddErrDebugFile ("WinMain : WSAAsyncSelect");
		free (ip_serveur);
		closesocket (socket_client);
		WSACleanup();
		return 0;
	}

	// Connection au serveur
	retour = connect (socket_client, (SOCKADDR *) &socket_serveur_sin, sizeof(socket_serveur_sin));

	// Boucle des messages
	while(GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Envoi des messages de fins aux threads enfants
	TerminateThread (hThread [0], 0);
	PostThreadMessage (ThreadClavierSourisId, WM_TERMINER_THREAD_CLAVIER_SOURIS, NULL, NULL);
	PostThreadMessage (ThreadFichierId, WM_TERMINER_THREAD_FICHIER, NULL, NULL);

	// Attente de la fin des threads enfants
	WaitForMultipleObjects (3, hThread, TRUE, INFINITE);

	// Fermeture du socket
	shutdown (socket_client, SD_BOTH);
	closesocket (socket_client);

	// Libération de la section critique
	DeleteCriticalSection (&sectionCritiqueEcran);

	// Déchargement des WSA
	WSACleanup();

	// Destruction des variables globales
	free (ip_serveur);

	// Fin du programme client
	return msg.wParam;
}

LRESULT CALLBACK ClientWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Variables locales
	OPENFILENAME OpenFileName;
	BOOL bOpen;
	FILE *src;
	char szFile[1024];
	szFile[0] = '\0';
	SECURITY_ATTRIBUTES lpSecurity;
	char* buffer;
	buffer = (char*)malloc(512);
	if(buffer==NULL)
		MessageBox(NULL,"ERREUR","Info",NULL);
	fichier *MyFile = (fichier*)malloc(sizeof(fichier));
	if(MyFile==NULL)
		MessageBox(NULL,"ERREUR","Info",NULL);
	char strFileName[256];
	int itmp0,itmp1;

	HANDLE hFile;
	int iFileSizeTmp;

	char buffer_reception [TAILLE_BUFFER];
	char buffer_emission [TAILLE_BUFFER];
	HDC hdc;
	PAINTSTRUCT ps;
	static HWND scv ;
	static HWND sch ;
	int retour;
	SCROLLINFO info_scrollbars;
	info_scrollbars.cbSize = sizeof(SCROLLINFO);
	info_scrollbars.fMask = SIF_TRACKPOS;
	static ParamThreadEcran pte;
	static bool afficher = false;

	static hscroll_value = 0;
	static vscroll_value = 0;

	static data * MyData; 
	static POINT pt;
	int x, y;
	x = 0;
	y = 0;

	RECT rectangle1, rectangle2;
	POINT coordonnees;

	// Traitement du message
	switch (message)
	{
		case WM_CREATE:
			MyData = new data();
			return TRUE;

		case WM_SIZE:
			largeur_fenetre = LOWORD(lParam);
			hauteur_fenetre = HIWORD(lParam);
			hscroll_max_range = infoServeur.largeurEcran - largeur_fenetre;
			vscroll_max_range = infoServeur.hauteurEcran - hauteur_fenetre;
			if (afficherScrollBars && !pleinEcran)
			{
				SetScrollRange(hWnd, SB_HORZ, 0, hscroll_max_range, true);
				SetScrollRange(hWnd, SB_VERT, 0, vscroll_max_range, true);
			}
			InvalidateRect(hWnd, NULL, false);
			break;

		case WM_PAINT:
			EnterCriticalSection (&sectionCritiqueEcran);
			hdc = BeginPaint(hWnd,&ps);
			if (afficher)
			{
				if (pleinEcran)
				{
					GetClientRect (hWnd, &rectangle1);
					char buffer [256];
					sprintf(buffer, "%d %d %d %d", rectangle1.right - rectangle1.left, rectangle1.bottom - rectangle1.top, infoServeur.largeurEcran, infoServeur.hauteurEcran);
					AddInfoDebugFile (buffer);
					StretchBlt (hdc, 0, 0, rectangle1.right - rectangle1.left, rectangle1.bottom - rectangle1.top, hdcScreen, 0, 0, infoServeur.largeurEcran, infoServeur.hauteurEcran, SRCCOPY);
				}
				else
				{
					if (afficherScrollBars)
						BitBlt (hdc, 0, 0, largeur_fenetre, hauteur_fenetre, hdcScreen, GetScrollPos(hWnd, SB_HORZ), GetScrollPos(hWnd, SB_VERT), SRCCOPY);
					else
						BitBlt (hdc, 0, 0, largeur_fenetre, hauteur_fenetre, hdcScreen, 0, 0, SRCCOPY);
				}
			}
			if ((afficherScrollBars) && (!pleinEcran))
			{
				hscroll_value = GetScrollPos(hWnd, SB_HORZ);
				vscroll_value = GetScrollPos(hWnd, SB_VERT);
			}
			EndPaint(hWnd,&ps);
			LeaveCriticalSection (&sectionCritiqueEcran);
			return 0;

		case WM_THREAD_ECRAN :
			AddInfoDebugFile ("ClientWndProc : WM_THREAD_ECRAN");
			afficher = true;
			InvalidateRect(hWnd, NULL, false);
			return 0;

		case WM_COMMAND :
			switch(LOWORD(wParam))
			{
				case ID_AFFICHAGE_PLEINCRAN :

					pleinEcran = true;

					EnableMenuItem (hMenu, ID_AFFICHAGE_FENETRE, MF_ENABLED);
					CheckMenuItem (hMenu, ID_AFFICHAGE_FENETRE, MF_UNCHECKED);

					EnableMenuItem (hMenu, ID_AFFICHAGE_PLEINCRAN, MF_DISABLED);
					CheckMenuItem (hMenu, ID_AFFICHAGE_PLEINCRAN, MF_CHECKED);

					GetWindowRect(hWnd, &dimensionsFenetre);

					SetWindowLong(hWnd,GWL_STYLE, WS_POPUP);
					SetWindowLong(hWnd,GWL_EXSTYLE,WS_EX_APPWINDOW|WS_EX_TOPMOST);

					SetMenu (hWnd, NULL);

					ShowScrollBar (hWnd, SB_HORZ, false);
					ShowScrollBar (hWnd, SB_VERT, false);

					refreshMouse = true;

					SetWindowPos(hWnd, HWND_TOPMOST,0,0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);

					UpdateWindow (hWnd);
					break;

				case ID_AFFICHAGE_FENETRE :

					pleinEcran = false;

					EnableMenuItem (hMenu, ID_AFFICHAGE_FENETRE, MF_DISABLED);
					CheckMenuItem (hMenu, ID_AFFICHAGE_FENETRE, MF_CHECKED);

					EnableMenuItem (hMenu, ID_AFFICHAGE_PLEINCRAN, MF_ENABLED);
					CheckMenuItem (hMenu, ID_AFFICHAGE_PLEINCRAN, MF_UNCHECKED);

					ChangeDisplaySettings (NULL,0);

					SetMenu (hWnd, hMenu);

					if (afficherScrollBars)
					{
						SetWindowLong(hWnd,GWL_STYLE,WS_OVERLAPPEDWINDOW);
						SetWindowLong(hWnd,GWL_EXSTYLE,WS_EX_APPWINDOW);

						ShowScrollBar (hWnd, SB_HORZ, true);
						ShowScrollBar (hWnd, SB_VERT, true);
					}
					else
					{
						SetWindowLong (hWnd, GWL_STYLE, WS_OVERLAPPED | WS_SYSMENU | WS_BORDER | WS_CAPTION);
						SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW);

						ShowScrollBar (hWnd, SB_HORZ, false);
						ShowScrollBar (hWnd, SB_VERT, false);

						SetWindowLong (hWnd, GWL_STYLE, WS_SYSMENU | WS_BORDER | WS_CAPTION);
					}

					SetWindowPos(hWnd, HWND_TOPMOST, dimensionsFenetre.left, dimensionsFenetre.top, dimensionsFenetre.right - dimensionsFenetre.left, dimensionsFenetre.bottom - dimensionsFenetre.top, SWP_SHOWWINDOW);

					InvalidateRect (NULL, NULL, false);
					break;

				case ID_DECONNEXION :
					DestroyWindow(hWnd);
					PostQuitMessage(0);
					break;

				case ALT_TAB_MENU:
					MyData->type =3; //envoi d'information de type clavier
					PostThreadMessage (ThreadClavierSourisId, WM_CLAVIER, (WPARAM) MyData, NULL);
					break;

				case ALT_SHIFT_TAB_MENU:
					MyData->type =4; //envoi d'information de type clavier
					PostThreadMessage (ThreadClavierSourisId, WM_CLAVIER, (WPARAM) MyData, NULL);
					break;

				case WINDOWS_MENU:
					MyData->type =5; //envoi d'information de type clavier
					PostThreadMessage (ThreadClavierSourisId, WM_CLAVIER, (WPARAM) MyData, NULL);
					break;

				case ALT_F4_MENU:
					MyData->type =6; //envoi d'information de type clavier
					PostThreadMessage (ThreadClavierSourisId, WM_CLAVIER, (WPARAM) MyData, NULL);
					break;

				case CTRL_ALT_SUPP_MENU:
					MyData->type =7; //envoi d'information de type clavier
					PostThreadMessage (ThreadClavierSourisId, WM_CLAVIER, (WPARAM) MyData, NULL);
					break;

				case ID_ENVOIFICHIER:
					//Si je reçoit un message clavier je l'envoi directement sur le reseau
					OpenFileName.lStructSize = sizeof(OPENFILENAME);
					OpenFileName.hwndOwner = hWnd;
					OpenFileName.hInstance = ghInstance;
					OpenFileName.lpstrFilter = "Fichier text *.txt";
					OpenFileName.lpstrCustomFilter = NULL;
					OpenFileName.nMaxCustFilter = 0;
					OpenFileName.nFilterIndex = 0;
					OpenFileName.lpstrFile = szFile;
					OpenFileName.nMaxFile = sizeof(szFile);
					OpenFileName.lpstrFileTitle = NULL;
					OpenFileName.nMaxFileTitle = 0;
					OpenFileName.lpstrInitialDir = NULL;
					OpenFileName.lpstrTitle = "Ouvrir";
					OpenFileName.nFileOffset = 0;
					OpenFileName.nFileExtension = 0;
					OpenFileName.lpstrDefExt = NULL;
					OpenFileName.lCustData = NULL;
					OpenFileName.lpfnHook = NULL;
					OpenFileName.lpTemplateName = NULL;
					OpenFileName.Flags = OFN_SHOWHELP | OFN_EXPLORER;

					bOpen = GetOpenFileName(&OpenFileName);
					
					lpSecurity.bInheritHandle = TRUE;
					lpSecurity.nLength = sizeof(SECURITY_ATTRIBUTES);
					lpSecurity.lpSecurityDescriptor = NULL;

					if(bOpen)
					{
						/* Récupération du nom court du fichier à envoyer */
						itmp0 = strlen(OpenFileName.lpstrFile);
						itmp1 = itmp0;
						while(OpenFileName.lpstrFile[itmp0-1] != '\\')
							itmp0--;
						for(int ind=0;ind<(itmp1 - itmp0);ind++)
							strFileName[ind] = OpenFileName.lpstrFile[itmp0+ind];
						strFileName[ind] = '\0';
						
						/* Récupération de la taille du fichier */
						hFile = CreateFile(OpenFileName.lpstrFile,GENERIC_READ,FILE_SHARE_READ,&lpSecurity,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
						MyFile->FileSize = GetFileSize(hFile,NULL);
						iFileSizeTmp = MyFile->FileSize;
						
						/* Ouverture du fichier avec un pointeur de fichier */
						src = fopen(OpenFileName.lpstrFile,"rb");
						if(src==NULL)
							MessageBox(NULL,"Erreur","Info",NULL);
						//InvalidateRect (hWnd, NULL, false);
						
						//Copie du nom du fichier dans la structure à envoyer
						strcpy(MyFile->FileName,OpenFileName.lpstrFile);
						
						int i =0;
						MyFile->Finish = 0;
						
						while(iFileSizeTmp > 0 )
						{
							//Décrémente la taille restante à envoyer
							iFileSizeTmp -= 512;

							//Si la taille est négative c'est que c'est le dernier paquet à envoyer
							if(iFileSizeTmp <= 0)
							{
								//Mise à jour du flag de fin de transfert
								MyFile->Finish = 1;
								fread(buffer,sizeof(char),iFileSizeTmp + 512,src);
								memcpy(MyFile->DataFile,buffer,iFileSizeTmp + 512);
								MyFile->DataSendSize = iFileSizeTmp + 512;
								strcpy(MyFile->FileName,strFileName);
								PostThreadMessage (ThreadFichierId, WM_FICHIER, (WPARAM) MyFile, NULL);
								
							}
							else
							{
								MyFile->Finish= 0;
								fread(buffer,sizeof(char),512,src);
								memcpy(MyFile->DataFile,buffer,512);
								MyFile->DataSendSize = 512;
								strcpy(MyFile->FileName,strFileName);
								PostThreadMessage (ThreadFichierId, WM_FICHIER, (WPARAM) MyFile, NULL);
								Sleep(200);
							}
						}
						MessageBox(NULL,"Fin de la copie du fichier","Info",NULL);
						free(MyFile);
						free(buffer);
						fclose(src);
					}
					break;
			}
			return FALSE;

			
		case WM_CLOSE:
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			return FALSE;

		case WM_DESTROY:
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			return FALSE;

		case WM_HSCROLL :
			switch (LOWORD(wParam))
			{
				// Clic sur flèche droite
				case SB_LINERIGHT :
					SetScrollPos(hWnd, SB_HORZ, GetScrollPos(hWnd, SB_HORZ) + HSCROLL_PETIT_PAS, true);
					InvalidateRect(hWnd, NULL, false);
					break;

				// Clic sur flèche gauche
				case SB_LINELEFT :
					SetScrollPos(hWnd, SB_HORZ, GetScrollPos(hWnd, SB_HORZ) - HSCROLL_PETIT_PAS, true);
					InvalidateRect(hWnd, NULL, false);
					break;

				// Clic sur zone à droite du curseur
				case SB_PAGELEFT :
					SetScrollPos(hWnd, SB_HORZ, GetScrollPos(hWnd, SB_HORZ) - HSCROLL_GRAND_PAS, true);
					InvalidateRect(hWnd, NULL, false);
					break;

				// Clic sur zone à gauche du curseur
				case SB_PAGERIGHT :
					SetScrollPos(hWnd, SB_HORZ, GetScrollPos(hWnd, SB_HORZ) + HSCROLL_GRAND_PAS, true);
					InvalidateRect(hWnd, NULL, false);
					break;

				// Déplacement manuel du curseur
				case SB_THUMBTRACK :
					GetScrollInfo(hWnd, SB_HORZ, &info_scrollbars);
					SetScrollPos(hWnd, SB_HORZ, info_scrollbars.nTrackPos, false);
					InvalidateRect(hWnd, NULL, false);
					break;
			}
			return TRUE;

		case WM_VSCROLL :
			switch (LOWORD(wParam))
			{
				// Clic sur flèche haut
				case SB_LINEUP :
					SetScrollPos(hWnd, SB_VERT, GetScrollPos(hWnd, SB_VERT) - VSCROLL_PETIT_PAS, true);
					InvalidateRect(hWnd, NULL, false);
					break;

				// Clic sur flèche bas
				case SB_LINEDOWN :
					SetScrollPos(hWnd, SB_VERT, GetScrollPos(hWnd, SB_VERT) + VSCROLL_PETIT_PAS, true);
					InvalidateRect(hWnd, NULL, false);
					break;

				// Clic sur zone en haut du curseur
				case SB_PAGEUP :
					SetScrollPos(hWnd, SB_VERT, GetScrollPos(hWnd, SB_VERT) - VSCROLL_GRAND_PAS, true);
					InvalidateRect(hWnd, NULL, false);
					break;

				// Clic sur zone en bas du curseur
				case SB_PAGEDOWN :
					SetScrollPos(hWnd, SB_VERT, GetScrollPos(hWnd, SB_VERT) +VSCROLL_GRAND_PAS, true);
					InvalidateRect(hWnd, NULL, false);
					break;

				// Déplacement manuel du curseur
				case SB_THUMBTRACK :
					GetScrollInfo(hWnd, SB_VERT, &info_scrollbars);
					SetScrollPos(hWnd, SB_VERT, info_scrollbars.nTrackPos, false);
					InvalidateRect(hWnd, NULL, false);
					break;
			}
			return TRUE;

		case WM_SOCKET_NOTIFY :
			AddInfoDebugFile ("ClientWndProc : WM_SOCKET_NOTIFY");

			switch (WSAGETSELECTEVENT (lParam))
			{
				case FD_CONNECT :
					AddInfoDebugFile ("ClientWndProc : FD_CONNECT");

					if (WSAGETSELECTERROR (lParam))
					{
						AddErrDebugFile ("ClientWndProc : FD_CONNECT");
						MessageBox(NULL, TEXT("Le serveur n'est pas disponible !"), TEXT("Client WRC"), MB_OK | MB_ICONERROR | MB_TOPMOST | MB_SETFOREGROUND);
						DestroyWindow(hWnd);
						PostQuitMessage(0);
						return TRUE;
					}
					else
					{
						sprintf(buffer_emission, "%s : %d %d %d %d %d %d", CODE_CONNECTION_REQUESTED, infoClient.methodeEcran, infoClient.methodeEcranOptions, infoClient.sleepLength, infoClient.modeTransmission, infoClient.modeTransmissionOptions, infoClient.limitToSystray);
						send(socket_client, buffer_emission, strlen(buffer_emission) + 1 , 0);
					}
					break;

				case FD_CLOSE :
					AddInfoDebugFile ("ClientWndProc : FD_CLOSE");
					shutdown (socket_client, 2);
					closesocket (socket_client);
					socket_client = NULL;
					MessageBox(hWnd, TEXT("Le serveur n'est pas disponible !"), TEXT("Client WRC"), MB_OK | MB_ICONERROR | MB_TOPMOST | MB_SETFOREGROUND);
					DestroyWindow(hWnd);
					PostQuitMessage(0);
					break;

				case FD_READ:
					AddInfoDebugFile ("ClientWndProc : FD_READ");

					// Réception des informations
					retour = recv(socket_client, buffer_reception, TAILLE_BUFFER, 0);
					if (retour == SOCKET_ERROR)
					{
						AddErrDebugFile ("ClientWndProc : FD_READ : recv");
						return TRUE;
					}

					if (strncmp (buffer_reception, "INFOSERVEUR : ", strlen("INFOSERVEUR : ")) == 0)
					{
						AddInfoDebugFile ("ClientWndProc : FD_READ : RESOLUTION");

						// Récupération de la résolution du serveur dans le message
						char partie1 [TAILLE_BUFFER], partie2 [TAILLE_BUFFER], partie3 [TAILLE_BUFFER];
						sscanf (buffer_reception, "INFOSERVEUR : %s %s %s %I64u %I64u", partie1, partie2, partie3, &(infoServeur.n), &(infoServeur.e));
						infoServeur.largeurEcran = atoi (partie1);
						infoServeur.hauteurEcran = atoi (partie2);
						infoServeur.password = atoi (partie3);

						if (infoServeur.largeurEcran < GetSystemMetrics(SM_CXSCREEN) - BORDURE_GAUCHE_DROITE)
						{
							if (infoServeur.hauteurEcran < GetSystemMetrics(SM_CYSCREEN) - BORDURE_HAUT_BAS)
							{
								afficherScrollBars = false;
								ShowScrollBar (hWnd, SB_HORZ, false);
								ShowScrollBar (hWnd, SB_VERT, false);
								SetWindowLong (hWnd, GWL_STYLE, WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX);
								GetWindowRect (hWnd, &rectangle1);
								GetClientRect (hWnd, &rectangle2);
								coordonnees.x = infoServeur.largeurEcran + (rectangle1.right - rectangle1.left) - (rectangle2.right - rectangle2.left) - 2;
								coordonnees.y = infoServeur.hauteurEcran + (rectangle1.bottom - rectangle1.top) - (rectangle2.bottom - rectangle2.top) - 2;
								MoveWindow (hWnd, GetSystemMetrics(SM_CXSCREEN) / 2 - coordonnees.x / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - coordonnees.y / 2, coordonnees.x, coordonnees.y, true);
								InvalidateRect (hWnd, NULL, true);
							}
						}

						if (infoServeur.password)
						{
							quitter = false;

							DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_PASSWORD), NULL, PasswordDialogProc);

							// Vérifions que l'utilisateur n'a pas choisi de quitter l'application
							if (quitter)
							{
								WSACleanup();
								PostQuitMessage (0);
								return 0;
							}

							n = infoServeur.n;
							e = infoServeur.e;
							unsigned __int64 cryptage = Crypter (password, strlen(password));
							sprintf(buffer_emission, "PASSWORD : %I64u", cryptage);
							AddInfoDebugFile (buffer_emission);
							retour = send (socket_client, buffer_emission, strlen (buffer_emission) + 1, 0);
						}
						else
							send(socket_client, CODE_INFOSERVEUR_NOPASSWORD_RECEIVED, strlen(CODE_INFOSERVEUR_NOPASSWORD_RECEIVED) + 1 , 0);
					}
					if (strcmp (buffer_reception, CODE_CONNECTION_ACCEPTED) == 0)
					{
						ShowWindow (hWnd, SW_SHOW);

						AddInfoDebugFile ("ClientWndProc : FD_READ : CODE_CONNECTION_ACCEPTED");

						hEventThreadEcran = CreateEvent(NULL, TRUE, FALSE, NULL);

						// Renseignement d'une structure ParamThreadEcran
						pte.hWnd = hWnd;
						pte.resolution.x = infoServeur.largeurEcran;
						pte.resolution.y = infoServeur.hauteurEcran;
						pte.pThreadClavierSourisId = &ThreadClavierSourisId;

						hThread [0] = CreateThread(NULL, NULL, DemarrerThreadEcran, (LPVOID) &pte, NULL, &ThreadEcranId);

						WaitForSingleObject(hEventThreadEcran,INFINITE);

						hEventThreadClavierSouris = CreateEvent(NULL, TRUE, FALSE, NULL);

						hThread [1] = CreateThread (NULL, NULL, DemarrerThreadClavierSouris, NULL, NULL, &ThreadClavierSourisId);

						WaitForSingleObject(hEventThreadClavierSouris,INFINITE);

						hEventThreadFichier = CreateEvent(NULL, TRUE, FALSE, NULL);
						
						hThread [2] = CreateThread (NULL, NULL, DemarrerThreadFichier, NULL, NULL, &ThreadFichierId);

						WaitForSingleObject(hEventThreadFichier,INFINITE);

						send(socket_client, CODE_CONNECTION_CONFIRMED, strlen(CODE_CONNECTION_CONFIRMED) + 1 , 0);

						return TRUE;
					}
					if (strcmp (buffer_reception, CODE_CONNECTION_REFUSED) == 0)
					{
						MessageBox(hWnd, TEXT("Mauvais mot de passe !"), TEXT("Client WRC"), MB_OK | MB_ICONWARNING | MB_TOPMOST | MB_SETFOREGROUND);
						DestroyWindow(hWnd);
						PostQuitMessage(0);
						return TRUE;
					}
					break;
			}
			return TRUE;

		/***** Message clavier à envoyer ******/

		//Appui sur une touche
		case WM_KEYDOWN:
			//Si je reçoit un message clavier je l'envoi directement sur le reseau
			MyData->type =1; //envoi d'information de type clavier
			MyData->donnee = wParam; //code de la touche
			PostThreadMessage (ThreadClavierSourisId, WM_CLAVIER, (WPARAM) MyData, NULL);
			return 0;

		case WM_SYSKEYDOWN:
			//Si je reçoit un message clavier je l'envoi directement sur le reseau
			MyData->type =1; //envoi d'information de type clavier
			MyData->donnee = wParam; //code de la touche
			PostThreadMessage (ThreadClavierSourisId, WM_CLAVIER, (WPARAM) MyData, NULL);
			return 0;

		//Relache une touche
		case WM_KEYUP:
			//Si je reçoit un message clavier je l'envoi directement sur le reseau
			MyData->type =2; //envoi d'information de type clavier
			MyData->donnee = wParam; //code de la touche
			PostThreadMessage (ThreadClavierSourisId, WM_CLAVIER, (WPARAM) MyData, NULL);
			return 0;

		case WM_SYSKEYUP:
			//Si je reçoit un message clavier je l'envoi directement sur le reseau
			MyData->type =2; //envoi d'information de type clavier
			MyData->donnee = wParam; //code de la touche
			PostThreadMessage (ThreadClavierSourisId, WM_CLAVIER, (WPARAM) MyData, NULL);
			return 0;

		/***** Message souris à envoyer ******/

		//Deplacement de la souris
		case WM_MOUSEMOVE:

			//Récupération des coordonnées actuelles de la souris
			x = LOWORD(lParam);
			y = HIWORD(lParam);

			if (pleinEcran)
			{
				if (y == 0 && x < 10 && !afficherMenu)
				{
					SetMenu (hWnd, hMenu);
					afficherMenu = true;
					InvalidateRect (hWnd, NULL, true);
				}

				if (y > 0 && afficherMenu)
				{
					SetMenu (hWnd, NULL);
					afficherMenu = false;
					UpdateWindow (hWnd);
				}
			}

			//S'il y a une difference de 5 pixel entre la position de la souris maintenant
			//et la precedente enregistré, alors on envoi les nouvelles coordonnées de souris

			if (pleinEcran)
			{
				if (refreshMouse)
				{
					//Préparation de la structure à envoyer
					MyData->type = 10; // type 10, donnée souris
					MyData->pt.x = infoServeur.largeurEcran * x / GetSystemMetrics(SM_CXSCREEN);
					MyData->pt.y = infoServeur.hauteurEcran * y / GetSystemMetrics(SM_CYSCREEN);
					pt.x =x;
					pt.y =y;

					//Envoi du paquet
					PostThreadMessage (ThreadClavierSourisId, WM_SOURIS, (WPARAM) MyData, NULL);
					refreshMouse = false;
				}
				else
				{
					if(abs(x-pt.x) > 10 || abs(y-pt.y) > 10 )
					{
						//Préparation de la structure à envoyer
						MyData->type = 10; // type 10, donnée souris
						MyData->pt.x = infoServeur.largeurEcran * x / GetSystemMetrics(SM_CXSCREEN);
						MyData->pt.y = infoServeur.hauteurEcran * y / GetSystemMetrics(SM_CYSCREEN);
						pt.x =x;
						pt.y =y;

						//Envoi du paquet
						PostThreadMessage (ThreadClavierSourisId, WM_SOURIS, (WPARAM) MyData, NULL);
					}
				}
			}
			else
			{
				if(abs(x-pt.x) > 10 || abs(y-pt.y) > 10 )
				{
					//Préparation de la structure à envoyer
					MyData->type = 10; // type 10, donnée souris
					MyData->pt.x = x + hscroll_value; //coordonnée de souris
					MyData->pt.y = y + vscroll_value;
					pt.x =x;
					pt.y =y;

					//Envoi du paquet
					PostThreadMessage (ThreadClavierSourisId, WM_SOURIS, (WPARAM) MyData, NULL);
				}
			}
			return 0;

		//Appui sur le bouton gauche de la souris
		case WM_LBUTTONDOWN:
			if(gcEtatSourisL == SOURIS_UP)
			{
				gcEtatSourisL = SOURIS_DOWN;
				AddInfoDebugFile("WM_LBUTTONDOWN");
				MyData->type =11; //Code des données
				if (pleinEcran)
				{
					MyData->pt.x = infoServeur.largeurEcran * LOWORD(lParam) / GetSystemMetrics(SM_CXSCREEN);
					MyData->pt.y = infoServeur.hauteurEcran * HIWORD(lParam) / GetSystemMetrics(SM_CYSCREEN);
				}
				else
				{
					MyData->pt.x =LOWORD(lParam) + hscroll_value;
					MyData->pt.y =HIWORD(lParam) + vscroll_value;
				}
				PostThreadMessage (ThreadClavierSourisId, WM_SOURIS, (WPARAM) MyData, NULL);
			}
			return 0;

		//Relache sur le bouton gauche de la souris
		case WM_LBUTTONUP:
			if(gcEtatSourisL == SOURIS_DOWN)
			{
				gcEtatSourisL = SOURIS_UP;
				AddInfoDebugFile("WM_LBUTTONUP");
				MyData->type =12; //Code des données
				if (pleinEcran)
				{
					MyData->pt.x = infoServeur.largeurEcran * LOWORD(lParam) / GetSystemMetrics(SM_CXSCREEN);
					MyData->pt.y = infoServeur.hauteurEcran * HIWORD(lParam) / GetSystemMetrics(SM_CYSCREEN);
				}
				else
				{
					MyData->pt.x =LOWORD(lParam) + hscroll_value;
					MyData->pt.y =HIWORD(lParam) + vscroll_value;
				}
				PostThreadMessage (ThreadClavierSourisId, WM_SOURIS, (WPARAM) MyData, NULL);
			}
			return 0;

		//Appui sur le bouton droit de la souris
		case WM_RBUTTONDOWN:
			if(gcEtatSourisR == SOURIS_UP)
			{
				gcEtatSourisR = SOURIS_DOWN;
				AddInfoDebugFile("WM_RBUTTONDOWN");
				MyData->type =13; //Code des données
				if (pleinEcran)
				{
					MyData->pt.x = infoServeur.largeurEcran * LOWORD(lParam) / GetSystemMetrics(SM_CXSCREEN);
					MyData->pt.y = infoServeur.hauteurEcran * HIWORD(lParam) / GetSystemMetrics(SM_CYSCREEN);
				}
				else
				{
					MyData->pt.x =LOWORD(lParam) + hscroll_value;
					MyData->pt.y =HIWORD(lParam) + vscroll_value;
				}
				PostThreadMessage (ThreadClavierSourisId, WM_SOURIS, (WPARAM) MyData, NULL);
			}
			return 0;

		//Relache sur le bouton droit de la souris
		case WM_RBUTTONUP:
			if(gcEtatSourisR == SOURIS_DOWN)
			{
				gcEtatSourisR = SOURIS_UP;
				AddInfoDebugFile("WM_RBUTTONUP");
				MyData->type =14; //Code des données
				if (pleinEcran)
				{
					MyData->pt.x = infoServeur.largeurEcran * LOWORD(lParam) / GetSystemMetrics(SM_CXSCREEN);
					MyData->pt.y = infoServeur.hauteurEcran * HIWORD(lParam) / GetSystemMetrics(SM_CYSCREEN);
				}
				else
				{
					MyData->pt.x =LOWORD(lParam) + hscroll_value;
					MyData->pt.y =HIWORD(lParam) + vscroll_value;
				}
				PostThreadMessage (ThreadClavierSourisId, WM_SOURIS, (WPARAM) MyData, NULL);
			}
			return 0;

		//Double clic bouton gauche
		case WM_LBUTTONDBLCLK:
			AddInfoDebugFile("WM_LBUTTONDBLCLK");
			MyData->type =15; //Code des données
			if (pleinEcran)
			{
				MyData->pt.x = infoServeur.largeurEcran * x / GetSystemMetrics(SM_CXSCREEN);
				MyData->pt.y = infoServeur.hauteurEcran * y / GetSystemMetrics(SM_CYSCREEN);
			}
			else
			{
				MyData->pt.x =x + hscroll_value;
				MyData->pt.y =y + vscroll_value;
			}
			PostThreadMessage (ThreadClavierSourisId, WM_SOURIS, (WPARAM) MyData, NULL);
			return 0;
	}
	
	return DefWindowProc(hWnd,message,wParam,lParam);
}
