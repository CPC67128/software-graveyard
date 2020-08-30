/***
 *** Projet WRC - Serveur WRC
 ***
 *** Aurélien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (école d'ingénieurs 3IL - promotion 2004)
 ***
 *** Fichier : exportation_image.cpp
 *** Description : 
 ***
 ***/

#include "include.h"

void * InitBitmapAndMemory(InfoBitmap * informations, HDC * hdcDest , HBITMAP * hbmp)
{
	static HDC hdcwindows;
	DWORD			dwNumColors; 
	void			*pBits;
	BITMAPINFO			bmpinfo;

	void * receive;

	//recuperation de l'image du bureau
	hdcwindows=GetDC(GetDesktopWindow());
	
	//recuperation du nombre de couleurs (apparement)
	if (GetDeviceCaps(hdcwindows, BITSPIXEL) <= 8)
		dwNumColors = 256;
	else
		dwNumColors = 0;

	//creation d'un contexte de peripherique en memoire
	if (!(*hdcDest = CreateCompatibleDC(hdcwindows)))
		return (0);

	//enregistrement d'informations sur le futur bitmap dans une structure facile a utiliser ( pour etre envoye a l'ordinateur distant)
	informations->Numcolors=dwNumColors;
	informations->width = GetDeviceCaps(hdcwindows, HORZRES);
	informations->height = GetDeviceCaps(hdcwindows, VERTRES);
	informations->PixelSize=24;// = (WORD)GetDeviceCaps(hdcwindows, BITSPIXEL);
	informations->UncompressedSize = informations->width*informations->height*(int)(informations->PixelSize)/8;

	//creation d'une entete de bitmap
	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = informations->width;
	bmpinfo.bmiHeader.biHeight = informations->height;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 24; //informations->PixelSize;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = 0;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = dwNumColors;
	bmpinfo.bmiHeader.biClrImportant = dwNumColors;

	//creation du bitmap
	*hbmp = CreateDIBSection(hdcwindows, &bmpinfo, DIB_PAL_COLORS, &pBits, NULL, 0);

	//selection des caracteristique du bitmap dans le contexte de peripherique
	SelectObject(*hdcDest, *hbmp);

	//allocation d'un tableau pour les pixels
	receive=(void*)malloc(informations->UncompressedSize);

	return receive;
}

void PutBitmapInMemory(InfoBitmap informations, HDC hdcDest , HBITMAP hbmp , void * receive )
{

	HDC	hdcwindows;

	//recuperation de l'image du bureau
	hdcwindows=GetDC(GetDesktopWindow());

	//copie du bureau dans le contexte de peripherique (et donc le bitmap)
	BitBlt(hdcDest, 0, 0,informations.width,informations.height, hdcwindows, 0, 0, SRCCOPY);
		
	//recuperation des pixels dans le tableau
	GetBitmapBits(hbmp,informations.UncompressedSize,receive);
	
}
