/***
 *** Projet WRC - Client WRC
 ***
 *** Aur�lien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (�cole d'ing�nieurs 3IL - promotion 2004)
 ***
 *** Fichier : types.h
 *** Description : d�finition des structures et des types de donn�es
 ***
 ***/

/***{ Types }***/

typedef
	struct ParamThreadEcran
	{
		HWND hWnd;
		POINT resolution;
		DWORD * pThreadClavierSourisId;
	}
ParamThreadEcran;

typedef
	struct _INFOSERVEUR
	{
		LONG largeurEcran;
		LONG hauteurEcran;
		BOOL password;
		unsigned __int64 n;
		unsigned __int64 e;
	}
INFOSERVEUR;

typedef
	struct _INFOCLIENT
	{
		DWORD methodeEcran;
		DWORD methodeEcranOptions;
		DWORD sleepLength;
		DWORD modeTransmission;
		DWORD modeTransmissionOptions;
		DWORD limitToSystray;
	}
INFOCLIENT;

typedef
	struct InformationsBitmap
	{
		DWORD Numcolors;
		long width;
		long height;
		unsigned short	PixelSize;
		int UncompressedSize;

	}
InfoBitmap;

typedef
	struct EcranTransversal
	{
		HWND hwnd;
		HDC hdcMemoire;
		InfoBitmap Bmp;
		int Continuer;

	}
Ecran;

typedef
	struct _data
	{
		char type;
		POINT pt;
		WPARAM donnee;

	}
data;

typedef
	struct _fichier
	{
		char FileName[128];
		char DataFile[512];
		int FileSize;
		int DataSendSize;
		int NumPaquet;
		char Finish;
	}
fichier;
