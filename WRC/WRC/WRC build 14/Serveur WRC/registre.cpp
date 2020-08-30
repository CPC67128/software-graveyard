/***
 *** Projet WRC - Serveur WRC
 ***
 *** Aurélien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (école d'ingénieurs 3IL - promotion 2004)
 ***
 *** Fichier : registre.cpp
 *** Description : fonctions permettant d'accéder au registre Windows
 ***
 ***/

#include "include.h"

/***[ EnregistrerPassword ]***/

bool EnregistrerPassword (char * szPassword)
{
	// Ajout d'une information dans le fichier de debug
	AddInfoDebugFile ("EnregistrerPassword : début");

	// Variables locales
	HKEY hk;
	DWORD dwDisposition;
	LONG lResult;

	// Création de la clé
	lResult = RegCreateKeyEx (HKEY_CURRENT_USER, "Software\\WRC", NULL, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hk, &dwDisposition);
	if (lResult)
	{
		AddErrDebugFile ("EnregistrerPassword : RegCreateKeyEx");
		return false;
	}

	// Ecriture de la valeur
	lResult = RegSetValueEx (hk, "Password", 0, REG_SZ, (BYTE *) szPassword, strlen(szPassword));
	if (lResult)
	{
		AddErrDebugFile ("EnregistrerPassword : RegSetValueEx");
		return false;
	}

	// Ajout d'une information dans le fichier de debug
	AddInfoDebugFile ("EnregistrerPassword : fin");

	// Fin de la fonction
	return true;
}

/***[ LirePassword ]***/

bool LirePassword (char ** buffer, DWORD taille_buffer)
{
	// Ajout d'une information dans le fichier de debug
	AddInfoDebugFile ("LirePassword : début");

	// Variables locales
	HKEY hk;
	LONG lResult;

	// Ouverture de la clé
	lResult = RegOpenKeyEx (HKEY_CURRENT_USER, "Software\\WRC", 0, KEY_QUERY_VALUE, &hk);
	if (lResult)
	{
		AddErrDebugFile ("LirePassword : RegOpenKeyEx");
		return false;
	}

	// Ouverture de la clé
	lResult = RegQueryValueEx (hk, "Password", 0, 0, (BYTE *) (*buffer), &taille_buffer);
	if (lResult)
	{
		AddErrDebugFile ("LirePassword : RegQueryValueEx");
		return false;
	}

	// Ajout d'une information dans le fichier de debug
	AddInfoDebugFile ("LirePassword : fin");

	// Fin de la fonction
	return true;
}

/***[ SupprimerPassword ]***/

bool SupprimerPassword ()
{
	// Ajout d'une information dans le fichier de debug
	AddInfoDebugFile ("SupprimerPassword : début");

	// Variables locales
	HKEY hk;
	LONG lResult;

	// Ouverture de la clé
	lResult = RegOpenKeyEx (HKEY_CURRENT_USER, "Software\\WRC", 0, KEY_ALL_ACCESS, &hk);

	// Ouverture de la clé
	lResult = RegDeleteValue(hk, "Password");

	// Ajout d'une information dans le fichier de debug
	AddInfoDebugFile ("SupprimerPassword : fin");

	// Fin de la fonction
	return true;
}

/***[ ActiveDesktop ]***/

int ActiveDesktop (bool set)
{
	CComQIPtr<IActiveDesktop> m_pIActiveDesktop;
	SHELLFLAGSTATE shfs;
	HRESULT m_hr;
	COMPONENTSOPT opt;
	int retour;

	CoInitialize(NULL);

	//initialisation de l'objet ATL
	m_hr=m_pIActiveDesktop.CoCreateInstance(
    CLSID_ActiveDesktop,
    NULL, 
    CLSCTX_INPROC_SERVER);

	if (m_hr==REGDB_E_CLASSNOTREG )
		MessageBox(NULL,"A specified class is not registered in the registration database. Also can indicate that the type of server you requested in the CLSCTX enumeration is not registered or the values for the server types in the registry are corrupt",NULL,NULL);
	if (m_hr==CLASS_E_NOAGGREGATION )
		MessageBox(NULL,"This class cannot be created as part of an aggregate",NULL,NULL);
	// If this bombs, you probably forgot to call call CoInitialize

	//test l'etat de active desktop
	SHGetSettings(&shfs, SSF_DESKTOPHTML);
	retour = shfs.fDesktopHTML;	// permet de renvoyer l'etat avant changement

	opt.dwSize = sizeof(opt);
	opt.fActiveDesktop = opt.fEnableComponents = set;
	m_hr = m_pIActiveDesktop->SetDesktopItemOptions(&opt,0);

	// Apply the changes
	m_pIActiveDesktop->ApplyChanges(AD_APPLY_REFRESH);

	return retour;
}

/***[ SupprimerArrierePlan ]***/

char * SupprimerArrierePlan (int * ad)
{
	char *szKEY = "Control Panel\\Desktop";
	char * Message;
	DWORD size;
	HKEY hk;	

	*ad=ActiveDesktop (false);
	Message=(char *)GlobalAlloc(GMEM_FIXED,255);
	if(RegOpenKeyEx(HKEY_CURRENT_USER, szKEY, 0, KEY_QUERY_VALUE, &hk)!=ERROR_SUCCESS)
	{
		MessageBox(NULL,TEXT("Erreur sur recherche de la cle dans la base de registre"),NULL,NULL);
		return NULL;
	}

	if(RegQueryValueEx(hk,"Wallpaper",0,0, (BYTE *)Message,&size)!=ERROR_SUCCESS)
	{
		MessageBox(NULL,TEXT("Erreur sur lecture dans la base de registre"),NULL,NULL);
		return NULL;
	}
	RegCloseKey(hk);

	MettreArrierePlan("", 0);
	return Message;
}

/***[ MettreArrierePlan ]***/

void MettreArrierePlan (char * fichier, int ad)
{
	if (ad)
		ActiveDesktop (true);
	if(fichier !=NULL)
		SystemParametersInfo(SPI_SETDESKWALLPAPER,0, fichier,0);
}
