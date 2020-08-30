/***
 *** Projet WRC - Serveur WRC
 ***
 *** Aurélien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (école d'ingénieurs 3IL - promotion 2004)
 ***
 *** Fichier : envoi_ecran.cpp
 *** Description : 
 ***
 ***/

#include "include.h"
extern INFOCLIENT infoClient;

int SendParLot(int sock, char * message, int size, int size_lot)
{
	AddInfoDebugFile ("SendParLot : début");

	int nb_lot,i;
	int taille_reste;
	char lu;
	int rep;

	nb_lot=size/size_lot;
	taille_reste=size%size_lot;

	for(i=0;i<nb_lot;i++)
	{
		rep=send(sock,&(message[i*size_lot]),size_lot,0);
		if(rep==SOCKET_ERROR)
		{
			AddInfoDebugFile ("SendParLot : fin (SOCKET_ERROR)");
			return rep;
		}
		rep=recv(sock,&lu,1,0);
		if(rep==SOCKET_ERROR)
		{
			AddInfoDebugFile ("SendParLot : fin (SOCKET_ERROR)");
			return rep;
		}
	}
	if(taille_reste!=0)
	{
		rep=send(sock,&(message[i*size_lot]),taille_reste,0);
		if(rep==SOCKET_ERROR)
		{
			AddInfoDebugFile ("SendParLot : fin (SOCKET_ERROR)");
			return rep;
		}
	}

	AddInfoDebugFile ("SendParLot : fin");

	return size;
}

int EgaliteEcran_xor(InfoBitmap informations_bmp, char * tab1,char * tab2, char * tab_xor, int taille_tableau)
{
	AddInfoDebugFile ("EgaliteEcran_xor : début");

	int i;
	int retour;
	APPBARDATA pData;
	static int limite;
	static int init=1;

	retour= TRUE;

	if (infoClient.limitToSystray)
	{
		if(init)
		{
			pData.cbSize = sizeof( PAPPBARDATA);
			SHAppBarMessage (ABM_GETTASKBARPOS, &pData);
			limite=(informations_bmp.height-pData.rc.top)*informations_bmp.width*informations_bmp.PixelSize;
			init=0;
		}
		for(i=0;i<taille_tableau;i++)
		{
			tab_xor[i]=tab1[i]^tab2[i];
			if ((i<limite) && (tab_xor[i]))
			{
				retour = FALSE;
			}
		}
	}
	else
		for(i=0;i<taille_tableau;i++)
		{
			tab_xor[i]=tab1[i]^tab2[i];
			if (tab_xor[i])
			{
				retour = FALSE;
			}
		}

	if (retour)
		AddInfoDebugFile ("EgaliteEcran_xor : fin (TRUE)");
	else
		AddInfoDebugFile ("EgaliteEcran_xor : fin (FALSE)");

	return retour;
}

int EgaliteEcran(InfoBitmap informations_bmp, char * tab1,char * tab2, int taille_tableau)
{
	AddInfoDebugFile ("EgaliteEcran : début");

	int i;
	APPBARDATA pData;
	static int limite;
	static int init=1;
	
	if (infoClient.limitToSystray)
	{
		if(init)
		{
			pData.cbSize = sizeof( PAPPBARDATA);
			SHAppBarMessage (ABM_GETTASKBARPOS, &pData);
			limite=(informations_bmp.height-pData.rc.top)*informations_bmp.width*informations_bmp.PixelSize;
			init=0;
		}
		for(i=0;i<limite;i++)
		{
			if (tab1[i]!=tab2[i])
			{
				AddInfoDebugFile ("EgaliteEcran : fin (FALSE)");
				return FALSE;
			}
		}
	}
	else
		for(i=0;i<taille_tableau;i++)
		{
			if (tab1[i]!=tab2[i])
			{
				AddInfoDebugFile ("EgaliteEcran : fin (FALSE)");
				return FALSE;
			}
		}

	AddInfoDebugFile ("EgaliteEcran : fin (TRUE)");

	return TRUE;
}

int EnvoieImage(int insock, HDC * hdcDest,HBITMAP * hbmp, char **tab, char ** tab_Nm1, char ** tab_xor,char ** tab_compresse, int initialiser)
{
	AddInfoDebugFile ("EnvoieImage : début");

	static InfoBitmap informations_bmp;	//structure contenant les informations pour creer le bmp 
	char * buff;

	int reponse;						// variable de test des codes retour
	unsigned long taille_compresse;		//variable temporaire servant a recuperer la taille du tableau apres compression

	//si c'est la premiere fois, on alloue la memoire utile
	if(initialiser)
	{
		*tab = (char *)InitBitmapAndMemory(&informations_bmp, hdcDest , hbmp);
		*tab_Nm1=(char *)malloc(informations_bmp.UncompressedSize);
		*tab_xor=(char *)malloc(informations_bmp.UncompressedSize);
		*tab_compresse=(char *)malloc(informations_bmp.UncompressedSize);	

		//envoie des informations
		reponse=send(insock,(char *)&informations_bmp,sizeof(InfoBitmap),0);
		if (reponse == SOCKET_ERROR)
		{
			AddInfoDebugFile ("EnvoieImage : fin (SOCKET_ERROR)");
			return 0;
		}
	}

	//mise en memoire de l'ecran
	PutBitmapInMemory(informations_bmp,*hdcDest,*hbmp ,*tab);

	if (initialiser)
	{
		if(infoClient.methodeEcran==METHODE_ECRAN_ZIP)
		{
			// Compression du tableau avant envoie	
			compress2((LPGZIP)(*tab_compresse),&taille_compresse,(LPGZIP)(*tab),informations_bmp.UncompressedSize,infoClient.methodeEcranOptions);
		}
		else	//infoClient.methodeEcran==METHODE_ECRAN_JPEG
		{
			taille_compresse = (unsigned long)jpeg_compress(*tab_compresse, *tab, informations_bmp.width, informations_bmp.height, informations_bmp.UncompressedSize, (infoClient.methodeEcranOptions + 1) * 10);
		}
	}
	else
	{	
		Sleep(infoClient.sleepLength);
		if(infoClient.methodeEcran==METHODE_ECRAN_ZIP)
		{
			if (EgaliteEcran_xor(informations_bmp, *tab,*tab_Nm1, *tab_xor, informations_bmp.UncompressedSize))
			{
				Sleep(1000);	//0,1 seconde
				AddInfoDebugFile ("EnvoieImage : fin (1)");
				return 1;
			}
			// Compression du tableau avant envoie
			compress2((LPGZIP)(*tab_compresse),&taille_compresse,(LPGZIP)(*tab_xor),informations_bmp.UncompressedSize,infoClient.methodeEcranOptions);
		}
		else //infoClient.methodeEcran==METHODE_ECRAN_JPEG
		{
			if (EgaliteEcran(informations_bmp, *tab,*tab_Nm1, informations_bmp.UncompressedSize))
			{
				Sleep(1000);	//0,1 seconde
				AddInfoDebugFile ("EnvoieImage : fin (1)");
				return 1;
			}
			taille_compresse = (unsigned long)jpeg_compress(*tab_compresse, *tab, informations_bmp.width, informations_bmp.height, informations_bmp.UncompressedSize, (infoClient.methodeEcranOptions + 1) * 10);
		}
	}

	//envoie de la taille du tableau compresse
	reponse=send(insock,(char *)&taille_compresse,sizeof(unsigned long),0);
	if (reponse == SOCKET_ERROR)
	{
		AddInfoDebugFile ("EnvoieImage : fin (SOCKET_ERROR)");
		return 0;
	}

	//envoie des donnees
	if ( infoClient.modeTransmission == MODE_TRANSMISSION_NORMAL)
		reponse=send(insock,*tab_compresse,taille_compresse,0);
	else
		reponse=SendParLot(insock,*tab_compresse,taille_compresse,infoClient.modeTransmissionOptions *1024);
	if (reponse == SOCKET_ERROR)
	{
		AddInfoDebugFile ("EnvoieImage : fin (SOCKET_ERROR)");
		return 0;
	}
	
	buff=*tab_Nm1;
	*tab_Nm1=*tab;
	*tab=buff;

	AddInfoDebugFile ("EnvoieImage : fin (1)");
	return 1;
}

void BoucleEnvoiImage(int insock)
{
	AddInfoDebugFile ("BoucleEnvoiImage : début");

	HDC hdcDest;					//contexte de peripherique memoire dans lequel on place l'image de l'ecran
	HBITMAP hbmp;					//Bmp servant a l'extraction du fond d'ecran
	char * tab, * tab_compresse = NULL, * tab_Nm1, *tab_xor;	//tableaux de bits contenant les informations de l'ecran
	MSG msg;
	int initialiser = 1;
	int continuer = 1;

	if (infoClient.methodeEcran == METHODE_ECRAN_ZIP)
	{
		switch (infoClient.methodeEcranOptions)
		{
			case 0 : infoClient.methodeEcranOptions = Z_DEFAULT_COMPRESSION; break;
			case 1 : infoClient.methodeEcranOptions = Z_BEST_SPEED; break;
			case 2 : infoClient.methodeEcranOptions = Z_BEST_COMPRESSION; break;
		}
	}
	while (continuer)
	{
		continuer = EnvoieImage((int) (insock), &hdcDest, &hbmp, &tab, &tab_Nm1,&tab_xor,&tab_compresse, initialiser);
		initialiser = 0;

		if (PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE) != NULL)
			if (msg.message == WM_TERMINER_THREAD_ECRAN)
			{
				AddInfoDebugFile ("BoucleEnvoiImage : réception de WM_TERMINER_THREAD_ECRAN");
				continuer = 0;
			}
	}

	free(tab);
	free(tab_Nm1);
	free(tab_xor);
	DeleteObject(hbmp);
	DeleteDC(hdcDest);

	AddInfoDebugFile ("BoucleEnvoiImage : fin");
}
