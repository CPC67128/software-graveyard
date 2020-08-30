/***
 *** Projet WRC - Serveur WRC
 ***
 *** Aurélien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (école d'ingénieurs 3IL - promotion 2004)
 ***
 *** Fichier : systray.cpp
 *** Description : fonctions de gestion de la barre système de Windows
 ***
 ***/

#include "include.h"

/***{ Variables globales }***/

NOTIFYICONDATA icone;

/***[ InitSysTrayIcon ]***/

BOOL InitSysTrayIcon (HWND hWnd, HICON hIcon, char * tooltip)
{
	// Initialisation de la structure NOTIFYICONDATA
    icone.cbSize = sizeof(icone);						// On alloue la taille nécessaire pour la structure
    icone.uID = SYSTRAY_UID;							// On lui donne un ID
	icone.hWnd = hWnd;									// On lui donne un handle
    icone.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;	// On lui indique les champs valables
    icone.uCallbackMessage = MYWM_NOTIFYICON;			// On lui indique qu'elle devra "écouter" son environnement (clique de souris, etc)
    icone.hIcon = hIcon;								// On lui donne le handle d'une icône
    strcpy (icone.szTip, tooltip);						// On lui indique le tooltip

	return TRUE;
}

/***[ AddIcon ]***/

BOOL AddIcon ()
{
	// Envoi du message correspondant au système
	int retour_shell;
	retour_shell = Shell_NotifyIcon(NIM_ADD, &icone);

	return retour_shell;
}

/***[ RemoveIcon ]***/

BOOL RemoveIcon ()
{
	// Envoi du message correspondant au système
	int retour_shell;
	retour_shell = Shell_NotifyIcon(NIM_DELETE, &icone);

	return retour_shell;
}

/***[ ChangeIcon ]***/

BOOL ChangeIcon (HICON hIcon)
{
	// Changement de l'icône dans la structure
    icone.hIcon = hIcon;

	// Envoi du message correspondant au système
	int retour_shell;
	retour_shell = Shell_NotifyIcon(NIM_MODIFY, &icone);

	return retour_shell;
}

/***[ ChangeTooltip ]***/

BOOL ChangeTooltip (char * tooltip)
{
	// Changement du texte du tooltip
	strcpy (icone.szTip, tooltip);

	// Envoi du message correspondant au système
	int retour_shell;
	retour_shell = Shell_NotifyIcon(NIM_MODIFY, &icone);

	return retour_shell;
}

/***[ ShowMenu ]***/

DWORD ShowMenu (HMENU hMenuPopup)
{
	// Détection de la position de la souris
	POINT position_souris;
	GetCursorPos (&position_souris);

	// Si on ne clique sur aucun choix du menu, il s'en va tout seul (permet d'éviter un bug Windows)
	SetForegroundWindow(icone.hWnd);

	// On appelle le menu, on demande que la valeur cliquée soit retournée (TPM_RETURNCMD)
	DWORD selection;
	selection = TrackPopupMenu(hMenuPopup, TPM_NONOTIFY | TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, position_souris.x - 90, position_souris.y, 0, icone.hWnd, NULL);
	return selection;
}
