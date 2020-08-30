/***
 *** Projet WRC - Client WRC
 ***
 *** Aur�lien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (�cole d'ing�nieurs 3IL - promotion 2004)
 ***
 *** Fichier : registre.cpp
 *** Description : fonctions permettant d'acc�der au registre Windows
 ***
 ***/

#include "include.h"

/***[ EnregistrerDerniereAdresseIP ]***/

bool EnregistrerDerniereAdresseIP (char * adresse_ip)
{
	// Ajout d'une information dans le fichier de debug
	AddInfoDebugFile ("EnregistrerDerniereAdresseIP : d�but");

	// Variables locales
	HKEY hk;
	DWORD dwDisposition;
	LONG lResult;

	// Cr�ation de la cl�
	lResult = RegCreateKeyEx (HKEY_CURRENT_USER, "Software\\WRC", NULL, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hk, &dwDisposition);
	if (lResult)
	{
		AddErrDebugFile ("EnregistrerDerniereAdresseIP : RegCreateKeyEx");
		return false;
	}

	// Ecriture de la valeur
	lResult = RegSetValueEx (hk, "Adresse IP", 0, REG_SZ, (BYTE *) adresse_ip, strlen(adresse_ip));
	if (lResult)
	{
		AddErrDebugFile ("EnregistrerDerniereAdresseIP : RegSetValueEx");
		return false;
	}

	// Ajout d'une information dans le fichier de debug
	AddInfoDebugFile ("EnregistrerDerniereAdresseIP : fin");

	// Fin de la fonction
	return true;
}

/***[ LireDerniereAdresseIP ]***/

bool LireDerniereAdresseIP (char ** buffer, DWORD taille_buffer)
{
	// Ajout d'une information dans le fichier de debug
	AddInfoDebugFile ("LireDerniereAdresseIP : d�but");

	// Variables locales
	HKEY hk;
	LONG lResult;

	// Ouverture de la cl�
	lResult = RegOpenKeyEx (HKEY_CURRENT_USER, "Software\\WRC", 0, KEY_QUERY_VALUE, &hk);
	if (lResult)
	{
		AddErrDebugFile ("LireDerniereAdresseIP : RegOpenKeyEx");
		return false;
	}

	// Ouverture de la cl�
	lResult = RegQueryValueEx (hk, "Adresse IP", 0, 0, (BYTE *) (*buffer), &taille_buffer);
	if (lResult)
	{
		AddErrDebugFile ("LireDerniereAdresseIP : RegQueryValueEx");
		return false;
	}

	// Ajout d'une information dans le fichier de debug
	AddInfoDebugFile ("LireDerniereAdresseIP : fin");

	// Fin de la fonction
	return true;
}

/***[ LireInfoClient ]***/

bool LireInfoClient (INFOCLIENT * infoClient)
{
	// Ajout d'une information dans le fichier de debug
	AddInfoDebugFile ("LireInfoClient : d�but");

	// Variables locales
	HKEY hk;
	LONG lResult;
	DWORD dwTemp;
	DWORD dwSize = sizeof (DWORD);

	// Ouverture de la cl�
	lResult = RegOpenKeyEx (HKEY_CURRENT_USER, "Software\\WRC", 0, KEY_QUERY_VALUE, &hk);

	// Lecture de la valeur correspondante � la m�thode de transmission des �crans choisie
	lResult = RegQueryValueEx (hk, "M�thode de transmission des �crans", 0, 0, (BYTE *) (&dwTemp), (LPDWORD) &dwSize);
	if (lResult)
	{
		// La valeur n'a pas pu �tre lue, il va falloir d�finir une valeur par d�faut
		dwTemp = METHODE_ECRAN_DEFAUT;
	}
	infoClient->methodeEcran = dwTemp;

	// Lecture de la valeur correspondante � la m�thode de transmission des �crans choisie
	lResult = RegQueryValueEx (hk, "Options de la m�thode de transmission des �crans", 0, 0, (BYTE *) (&dwTemp), (LPDWORD) &dwSize);
	if (lResult)
	{
		// La valeur n'a pas pu �tre lue, il va falloir d�finir une valeur par d�faut
		dwTemp = OPTIONS_METHODE_ECRAN_DEFAUT;
	}
	infoClient->methodeEcranOptions = dwTemp;

	// Lecture de la valeur correspondante au temps d'attente entre chaque envois d'images
	lResult = RegQueryValueEx (hk, "D�lai d'attente", 0, 0, (BYTE *) (&dwTemp), (LPDWORD) &dwSize);
	if (lResult)
	{
		// La valeur n'a pas pu �tre lue, il va falloir d�finir une valeur par d�faut
		dwTemp = SLEEP_LENGTH_DEFAUT;
	}
	infoClient->sleepLength = dwTemp;

	// Lecture de la valeur correspondante au mode de transmission
	lResult = RegQueryValueEx (hk, "Mode de transmission", 0, 0, (BYTE *) (&dwTemp), (LPDWORD) &dwSize);
	if (lResult)
	{
		// La valeur n'a pas pu �tre lue, il va falloir d�finir une valeur par d�faut
		dwTemp = MODE_TRANSMISSION_DEFAUT;
	}
	infoClient->modeTransmission = dwTemp;

	// Lecture de la valeur correspondante au temps d'attente entre chaque envois d'images
	lResult = RegQueryValueEx (hk, "Options du mode de transmission", 0, 0, (BYTE *) (&dwTemp), (LPDWORD) &dwSize);
	if (lResult)
	{
		// La valeur n'a pas pu �tre lue, il va falloir d�finir une valeur par d�faut
		dwTemp = OPTIONS_MODE_TRANSMISSION_DEFAUT;
	}
	infoClient->modeTransmissionOptions = dwTemp;

	// Lecture de la valeur correspondante � la prise en compte des modifications de la barre Windows
	lResult = RegQueryValueEx (hk, "Prise en compte des modifications de la barre Windows", 0, 0, (BYTE *) (&dwTemp), (LPDWORD) &dwSize);
	if (lResult)
	{
		// La valeur n'a pas pu �tre lue, il va falloir d�finir une valeur par d�faut
		dwTemp = LIMIT_TO_SYSTRAY_DEFAUT;
	}
	infoClient->limitToSystray = dwTemp;

	// Ajout d'une information dans le fichier de debug
	AddInfoDebugFile ("LireInfoClient : fin");

	// Fin de la fonction
	return true;
}

/***[ EnregistrerInfoClient ]***/

bool EnregistrerInfoClient (INFOCLIENT * infoClient)
{
	// Ajout d'une information dans le fichier de debug
	AddInfoDebugFile ("EnregistrerInfoClient : d�but");

	// Variables locales
	HKEY hk;
	LONG lResult;
	DWORD dwSize = sizeof (DWORD);
	DWORD dwDisposition;

	// Ouverture de la cl�
	lResult = RegCreateKeyEx (HKEY_CURRENT_USER, "Software\\WRC", NULL, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hk, &dwDisposition);

	// Enregistrement de la valeur correspondante � la m�thode de transmission des �crans choisie
	lResult = RegSetValueEx (hk, "M�thode de transmission des �crans", 0, REG_DWORD, (BYTE *) &(infoClient->methodeEcran), dwSize);

	// Enregistrement de la valeur correspondante � la m�thode de transmission des �crans choisie
	lResult = RegSetValueEx (hk, "Options de la m�thode de transmission des �crans", 0, REG_DWORD, (BYTE *) &(infoClient->methodeEcranOptions), dwSize);

	// Enregistrement de la valeur correspondante au temps d'attente entre chaque envois d'images
	lResult = RegSetValueEx (hk, "D�lai d'attente", 0, REG_DWORD, (BYTE *) &(infoClient->sleepLength), dwSize);

	// Enregistrement de la valeur correspondante au mode de transmission
	lResult = RegSetValueEx (hk, "Mode de transmission", 0, REG_DWORD, (BYTE *) &(infoClient->modeTransmission), dwSize);

	// Enregistrement de la valeur correspondante au temps d'attente entre chaque envois d'images
	lResult = RegSetValueEx (hk, "Options du mode de transmission", 0, REG_DWORD, (BYTE *) &(infoClient->modeTransmissionOptions), dwSize);

	// Enregistrement de la valeur correspondante � la prise en compte des modifications de la barre Windows
	lResult = RegSetValueEx (hk, "Prise en compte des modifications de la barre Windows", 0, REG_DWORD, (BYTE *) &(infoClient->limitToSystray), dwSize);

	// Ajout d'une information dans le fichier de debug
	AddInfoDebugFile ("EnregistrerInfoClient : fin");

	// Fin de la fonction
	return true;
}
