/***
 *** Projet WRC - Client WRC
 ***
 *** Aurélien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (école d'ingénieurs 3IL - promotion 2004)
 ***
 *** Fichier : connexion.cpp
 *** Description : procédures des fenêtres de la phase de pré-connexion
 ***
 ***/

#include "include.h"

/***{ Variables globales }***/

extern INFOCLIENT infoClient;
extern char * ip_serveur;
extern bool quitter;
extern char * password;
extern SOCKADDR_IN socket_serveur_sin;

/***[ ConnexionDialogProc ]***/

BOOL CALLBACK ConnexionDialogProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Variables locales
	char buffer [TAILLE_BUFFER];
	unsigned int indice;

	// Traitement du message
	switch (message)
	{
		case WM_INITDIALOG :
			// Ajout de l'icône WRC à la fenêtre
			SendMessage (hWnd, WM_SETICON, (WPARAM) ICON_SMALL, LPARAM ((HICON) LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_WRC), IMAGE_ICON, 16, 16, 0)));

			// Ajout de l'adresse IP dans la zone d'édition prévue à cet effet
			SendMessage (GetDlgItem (hWnd, IDC_IP), WM_SETTEXT, TAILLE_BUFFER, (LPARAM) ip_serveur);

			// Ajout des options pour la compression ZLIB avec XOR
			SendMessage (GetDlgItem (hWnd, IDC_ZIP_OPTIONS), CB_ADDSTRING, (WPARAM) NULL, (LPARAM) TEXT("Normale"));
			SendMessage (GetDlgItem (hWnd, IDC_ZIP_OPTIONS), CB_ADDSTRING, (WPARAM) NULL, (LPARAM) TEXT("Rapide"));
			SendMessage (GetDlgItem (hWnd, IDC_ZIP_OPTIONS), CB_ADDSTRING, (WPARAM) NULL, (LPARAM) TEXT("Meilleure"));

			// Ajout des options pour la compression JPEG
			for (indice = 10; indice <= 100; indice += 10)
			{
				sprintf(buffer, "%d", indice);
				SendMessage (GetDlgItem (hWnd, IDC_JPEG_OPTIONS), CB_ADDSTRING, (WPARAM) NULL, (LPARAM) TEXT(buffer));
			}

			// Sélection de la méthode enregistrée dans le registre
			if (infoClient.methodeEcran == METHODE_ECRAN_ZIP)
			{
				SendMessage (GetDlgItem (hWnd, IDC_ZIP), BM_SETCHECK, (WPARAM) MF_CHECKED, (LPARAM) NULL);
				SendMessage (GetDlgItem (hWnd, IDC_JPEG), BM_SETCHECK, (WPARAM) MF_UNCHECKED, (LPARAM) NULL);
				SendMessage (GetDlgItem (hWnd, IDC_ZIP_OPTIONS), CB_SETCURSEL, (WPARAM) infoClient.methodeEcranOptions, (LPARAM) NULL);
				EnableWindow (GetDlgItem (hWnd, IDC_ZIP_OPTIONS), true);
				SendMessage (GetDlgItem (hWnd, IDC_JPEG_OPTIONS), CB_SETCURSEL, (WPARAM) OPTIONS_METHODE_ECRAN_JPEG_DEFAUT, (LPARAM) NULL);
				EnableWindow (GetDlgItem (hWnd, IDC_JPEG_OPTIONS), false);
			}
			else
			{
				SendMessage (GetDlgItem (hWnd, IDC_ZIP), BM_SETCHECK, (WPARAM) MF_UNCHECKED, (LPARAM) NULL);
				SendMessage (GetDlgItem (hWnd, IDC_JPEG), BM_SETCHECK, (WPARAM) MF_CHECKED, (LPARAM) NULL);
				SendMessage (GetDlgItem (hWnd, IDC_JPEG_OPTIONS), CB_SETCURSEL, (WPARAM) infoClient.methodeEcranOptions, (LPARAM) NULL);
				EnableWindow (GetDlgItem (hWnd, IDC_JPEG_OPTIONS), true);
				SendMessage (GetDlgItem (hWnd, IDC_ZIP_OPTIONS), CB_SETCURSEL, (WPARAM) OPTIONS_METHODE_ECRAN_ZIP_DEFAUT, (LPARAM) NULL);
				EnableWindow (GetDlgItem (hWnd, IDC_ZIP_OPTIONS), false);
			}

			// Ajout des délais d'attente possible
			for (indice = 0; indice <= 5000; indice += 250)
			{
				sprintf(buffer, "%d", indice);
				SendMessage (GetDlgItem (hWnd, IDC_SLEEP_LENGTH), CB_ADDSTRING, (WPARAM) NULL, (LPARAM) TEXT(buffer));
				if (indice == infoClient.sleepLength)
					SendMessage (GetDlgItem (hWnd, IDC_SLEEP_LENGTH), CB_SETCURSEL, (WPARAM) indice / 250, (LPARAM) NULL);
			}

			// Sélection ou non de la prise en compte des modifications de la barre Windows
			if (infoClient.limitToSystray == 0)
				SendMessage (GetDlgItem (hWnd, IDC_LIMIT_TO_SYSTRAY), BM_SETCHECK, (WPARAM) MF_CHECKED, (LPARAM) NULL);
			else
				SendMessage (GetDlgItem (hWnd, IDC_LIMIT_TO_SYSTRAY), BM_SETCHECK, (WPARAM) MF_UNCHECKED, (LPARAM) NULL);

			// Sélection du mode de transmission enregistré dans le registre
			if (infoClient.modeTransmission == MODE_TRANSMISSION_NORMAL)
			{
				SendMessage (GetDlgItem (hWnd, IDC_MODE_NORMAL), BM_SETCHECK, (WPARAM) MF_CHECKED, (LPARAM) NULL);
				SendMessage (GetDlgItem (hWnd, IDC_MODE_LOTS), BM_SETCHECK, (WPARAM) MF_UNCHECKED, (LPARAM) NULL);
				EnableWindow (GetDlgItem (hWnd, IDC_TAILLE_LOTS), false);
			}
			else
			{
				SendMessage (GetDlgItem (hWnd, IDC_MODE_NORMAL), BM_SETCHECK, (WPARAM) MF_UNCHECKED, (LPARAM) NULL);
				SendMessage (GetDlgItem (hWnd, IDC_MODE_LOTS), BM_SETCHECK, (WPARAM) MF_CHECKED, (LPARAM) NULL);
				EnableWindow (GetDlgItem (hWnd, IDC_TAILLE_LOTS), true);
			}

			// Ajout des tailles de lots possibles
			for (indice = 2; indice <= 40; indice += 2)
			{
				sprintf(buffer, "%d", indice);
				SendMessage (GetDlgItem (hWnd, IDC_TAILLE_LOTS), CB_ADDSTRING, (WPARAM) NULL, (LPARAM) TEXT(buffer));
				if (indice == infoClient.modeTransmissionOptions)
					SendMessage (GetDlgItem (hWnd, IDC_TAILLE_LOTS), CB_SETCURSEL, (WPARAM) (indice / 2) - 1, (LPARAM) NULL);
			}

			return TRUE;

		case WM_COMMAND :
			switch (LOWORD(wParam))
			{
				case IDOK:
					EndDialog(hWnd, 0);
					SendMessage(GetDlgItem(hWnd, IDC_IP), WM_GETTEXT, TAILLE_BUFFER, (LPARAM) ip_serveur);

					if (SendMessage (GetDlgItem (hWnd, IDC_ZIP), BM_GETCHECK, (WPARAM) NULL, (LPARAM) NULL) == BST_CHECKED)
					{
						infoClient.methodeEcran = METHODE_ECRAN_ZIP;
						infoClient.methodeEcranOptions = SendMessage (GetDlgItem(hWnd, IDC_ZIP_OPTIONS), CB_GETCURSEL, (WPARAM) NULL, (LPARAM) NULL);
					}
					else
					{
						infoClient.methodeEcran = METHODE_ECRAN_JPEG;
						infoClient.methodeEcranOptions = SendMessage(GetDlgItem(hWnd, IDC_JPEG_OPTIONS), CB_GETCURSEL, (WPARAM) NULL, (LPARAM) NULL);
					}
					SendMessage (GetDlgItem(hWnd, IDC_SLEEP_LENGTH), WM_GETTEXT, (WPARAM) TAILLE_BUFFER, (LPARAM) buffer);
					infoClient.sleepLength = atoi (buffer);

					if (SendMessage (GetDlgItem (hWnd, IDC_LIMIT_TO_SYSTRAY), BM_GETCHECK, (WPARAM) NULL, (LPARAM) NULL) == BST_CHECKED)
						infoClient.limitToSystray = 0;
					else
						infoClient.limitToSystray = 1;

					if (SendMessage (GetDlgItem (hWnd, IDC_MODE_NORMAL), BM_GETCHECK, (WPARAM) NULL, (LPARAM) NULL) == BST_CHECKED)
					{
						infoClient.modeTransmission = MODE_TRANSMISSION_NORMAL;
					}
					else
					{
						infoClient.modeTransmission = MODE_TRANSMISSION_LOTS;
						infoClient.modeTransmissionOptions = 2 + 2 * SendMessage (GetDlgItem(hWnd, IDC_TAILLE_LOTS), CB_GETCURSEL, (WPARAM) NULL, (LPARAM) NULL);
					}
					return TRUE;

				case IDCANCEL:
					EndDialog(hWnd, 0);
					quitter = true;
					return TRUE;

				case IDC_JPEG :
					SendMessage (GetDlgItem (hWnd, IDC_ZIP), BM_SETCHECK, (WPARAM) MF_UNCHECKED, (LPARAM) NULL);
					SendMessage (GetDlgItem (hWnd, IDC_JPEG), BM_SETCHECK, (WPARAM) MF_CHECKED, (LPARAM) NULL);
					EnableWindow (GetDlgItem (hWnd, IDC_ZIP_OPTIONS), false);
					EnableWindow (GetDlgItem (hWnd, IDC_JPEG_OPTIONS), true);
					return TRUE;

				case IDC_ZIP :
					SendMessage (GetDlgItem (hWnd, IDC_ZIP), BM_SETCHECK, (WPARAM) MF_CHECKED, (LPARAM) NULL);
					SendMessage (GetDlgItem (hWnd, IDC_JPEG), BM_SETCHECK, (WPARAM) MF_UNCHECKED, (LPARAM) NULL);
					EnableWindow (GetDlgItem (hWnd, IDC_ZIP_OPTIONS), true);
					EnableWindow (GetDlgItem (hWnd, IDC_JPEG_OPTIONS), false);
					return TRUE;

				case IDC_MODE_NORMAL :
					SendMessage (GetDlgItem (hWnd, IDC_MODE_NORMAL), BM_SETCHECK, (WPARAM) MF_CHECKED, (LPARAM) NULL);
					SendMessage (GetDlgItem (hWnd, IDC_MODE_LOTS), BM_SETCHECK, (WPARAM) MF_UNCHECKED, (LPARAM) NULL);
					EnableWindow (GetDlgItem (hWnd, IDC_TAILLE_LOTS), false);
					return TRUE;

				case IDC_MODE_LOTS :
					SendMessage (GetDlgItem (hWnd, IDC_MODE_NORMAL), BM_SETCHECK, (WPARAM) MF_UNCHECKED, (LPARAM) NULL);
					SendMessage (GetDlgItem (hWnd, IDC_MODE_LOTS), BM_SETCHECK, (WPARAM) MF_CHECKED, (LPARAM) NULL);
					EnableWindow (GetDlgItem (hWnd, IDC_TAILLE_LOTS), true);
					return TRUE;
			}
			break;
	}
	return FALSE;
}

/***[ PasswordDialogProc ]***/

BOOL CALLBACK PasswordDialogProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char buffer [TAILLE_BUFFER];

	switch (message)
	{
		case WM_INITDIALOG :
			SendMessage (hWnd, WM_SETICON, (WPARAM) ICON_SMALL, LPARAM ((HICON) LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_WRC), IMAGE_ICON, 16, 16, 0)));
			return TRUE;
		case WM_COMMAND :
			switch (LOWORD(wParam))
			{
				case IDOK:
					EndDialog(hWnd, 0);
					SendMessage(GetDlgItem(hWnd, IDC_PASSWORD), WM_GETTEXT, TAILLE_BUFFER, (LPARAM) buffer);
					password = algorithme_sha (buffer);
					return TRUE;
				case IDCANCEL:
					EndDialog(hWnd, 0);
					quitter = true;
					return TRUE;
			}
			break;
	}
	return FALSE;
}

/***[ VerifierIP ]***/

bool VerifierIP ()
{
	// Déclarations des variables
	char buffer_temp [TAILLE_BUFFER];
	int c1, c2, c3, c4;
	int retour;

	// Vérification de la structure syntaxique de l'adresse IP
	retour = sscanf (ip_serveur, "%d.%d.%d.%d%s", &c1, &c2, &c3, &c4, buffer_temp);
	if (retour != 4)
		return false;

	// Vérification de la viabilité des composantes de l'adresse IP
	if (c1 > 255 || c1 < 0)
		return false;
	if (c2 > 255 || c2 < 0)
		return false;
	if (c3 > 255 || c3 < 0)
		return false;
	if (c4 > 255 || c4 < 0)
		return false;

	// Vérification de la viabilité de l'adresse IP
	if (strcmp(ip_serveur, "127.0.0.1") == 0)
		return false;
	if (strcmp(ip_serveur, "255.255.255.255") == 0)
		return false;
	if (c1 >= 218 && c1 <= 223)
		return false;
	if (c1 >= 240 && c1 <= 255)
		return false;
	if (c1 >= 224 && c1 <= 239)
		return false;

	return true;
}

/***[ ResolutionAdresseIP ]***/

bool ResolutionAdresseIP ()
{
	// Variables locales
	struct hostent *host;

	// Création de la structure SOCKADDR_IN
	socket_serveur_sin.sin_family = AF_INET;

	socket_serveur_sin.sin_port = htons(PORT_SERVEUR);

	socket_serveur_sin.sin_addr.s_addr = inet_addr(ip_serveur);
	if (socket_serveur_sin.sin_addr.s_addr == -1)
	{
		host = gethostbyname(ip_serveur);
		if (host != NULL)
		{
			socket_serveur_sin.sin_addr.s_addr = *((unsigned long *) host->h_addr);
		}
		else
			return false;
	}
	else
		if (!VerifierIP ())
			return false;

	return true;
}
