/***
 *** Projet WRC - Client WRC
 ***
 *** Aurélien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (école d'ingénieurs 3IL - promotion 2004)
 ***
 *** Fichier : reception_ecran.cpp
 *** Description : 
 ***
 ***/

#include "include.h"

extern HDC hdcScreen;
extern HBITMAP hdcScreenBmp;
extern CRITICAL_SECTION sectionCritiqueEcran;
extern INFOCLIENT infoClient;

int RecvParLot(int sock, char * message, int size, int size_lot)
{
	int nb_lot,i;
	int taille_reste;
	char lu='*';
	int rep;

	nb_lot=size/size_lot;
	taille_reste=size%size_lot;

	for(i=0;i<nb_lot;i++)
	{
		rep=recv(sock,&(message[i*size_lot]),size_lot,0);
		if(rep==SOCKET_ERROR)
			return rep;
		rep=send(sock,&lu,1,0);
		if(rep==SOCKET_ERROR)
			return rep;
	}
	if(taille_reste!=0)
	{
		rep=recv(sock,&(message[i*size_lot]),taille_reste,0);
		if(rep==SOCKET_ERROR)
			return rep;
	}
	return size;
}

void CalculXor(char *tab1, char * tab2, int taille)
{
	int i;
	for (i=0;i<taille;i++)
	{
		tab1[i]=tab1[i]^tab2[i];
	}
}

HDC ReceptionImage(InfoBitmap * informations_bmp, int sock, char ** tab, char ** tab_xor, char ** tab_compresse)
{

	int lg;
	unsigned long Taille_decompresse;
	int Taille_compresse;
	static int initialiser=1;
	int w,h;


//traitements faits seulement une fois
	if(initialiser)
	{
		//reception des informations sur le bitmap
		lg=recv(sock, (char *)informations_bmp,sizeof(InfoBitmap),0);
		if (lg == SOCKET_ERROR)
		{
			AddErrDebugFile ("Ecran_Affichage : recv");
			return 0;
		}

//creation du bitmap qui recevra l'imprime ecran serveur
		EnterCriticalSection (&sectionCritiqueEcran);
		hdcScreen = InitBitmap(*informations_bmp,&hdcScreenBmp);
		LeaveCriticalSection (&sectionCritiqueEcran);

		//allocation des tableaux qui contiendront les couleurs transmisent par le serveur
		*tab=(char *)malloc(informations_bmp->UncompressedSize);
		*tab_xor=(char *)malloc(informations_bmp->UncompressedSize);
		*tab_compresse=(char *)malloc(informations_bmp->UncompressedSize);
	}

//reception de la taille du bitmap compressé
	lg=recv(sock, (char *)(&Taille_compresse),sizeof(unsigned long),0);
	if (lg == SOCKET_ERROR)
	{
		AddErrDebugFile ("Ecran_Affichage : recv");
		return 0;
	}

	if (Taille_compresse == 0)
	{
		AddErrDebugFile ("Ecran_Affichage : TailleCompresse == 0");
		return 0;
	}

//reception des informations
	if (infoClient.modeTransmission==MODE_TRANSMISSION_NORMAL)
			lg=recv(sock, (char *)(*tab_compresse),Taille_compresse,0);
	else
		lg=RecvParLot(sock,(char *)(*tab_compresse),Taille_compresse,infoClient.modeTransmissionOptions*1024);
	if (lg == SOCKET_ERROR)
	{
		char erreur[255];
		sprintf(erreur,"Ecran_Affichage : recv %d",WSAGetLastError());
		AddErrDebugFile (erreur);
		return 0;
	}

	EnterCriticalSection (&sectionCritiqueEcran);
	// Décompression du tableau aprés réception	
	if (initialiser)
	{
		initialiser=0;
		if (infoClient.methodeEcran==METHODE_ECRAN_ZIP)
		{
			if (uncompress((LPGZIP)(*tab),&Taille_decompresse,(LPGZIP)(*tab_compresse),(unsigned long)(Taille_compresse))==Z_DATA_ERROR)
			{
				AddErrDebugFile ("Ecran_Affichage : erreur décompression");
			}
		}
		else
		{
			jpeg_decompress((unsigned char *)(*tab), (unsigned char *)(*tab_compresse), (int)(Taille_compresse), &w, &h);
		}
	}
	else
	{	
		if (infoClient.methodeEcran==METHODE_ECRAN_ZIP)
		{
			if (uncompress((LPGZIP)(*tab_xor),&Taille_decompresse,(LPGZIP)(*tab_compresse),(unsigned long)(Taille_compresse))!=Z_OK)
			{
				AddErrDebugFile ("Ecran_Affichage : erreur décompression");
			}
			CalculXor(*tab, *tab_xor,informations_bmp->UncompressedSize);
		}
		else
		{
			jpeg_decompress((unsigned char *)(*tab), (unsigned char *)(*tab_compresse), (int)(Taille_compresse), &w, &h);
		}
	}
	
	AddEcranToBitmap(hdcScreenBmp,*informations_bmp,*tab);	
	LeaveCriticalSection (&sectionCritiqueEcran);

	return hdcScreen;
}



void BoucleReceptionImage(Ecran * informations, int sock)
{
	char * tab_compresse, * tab, * tab_xor;

	informations->Continuer=1;

	while(informations->Continuer)
	{	
		AddInfoDebugFile("Ecran : début");
		informations->hdcMemoire = ReceptionImage(&(informations->Bmp), sock, &tab,&tab_xor, &tab_compresse);
		AddInfoDebugFile("Ecran : fin");
		if (informations->hdcMemoire == 0)
			informations->Continuer = false;
		else
		{
			SendMessage (informations->hwnd, WM_THREAD_ECRAN, NULL, NULL);
		}
	}

	DeleteDC(informations->hdcMemoire);

	free(tab_compresse);
	free(tab_xor);
	free(tab);
}

