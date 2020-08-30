/***
 *** Projet WRC - Client WRC
 ***
 *** Aurélien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (école d'ingénieurs 3IL - promotion 2004)
 ***
 *** Fichier : importation_images.cpp
 *** Description : 
 ***
 ***/

#include "include.h"

HDC InitBitmap(InfoBitmap informations, HBITMAP * hbmp)
{
	BITMAPINFO			bmpinfo;
	void				*pBits;
	HDC					hdcMem;

	//on creer un contexte de peripherique en memoire
	hdcMem=CreateCompatibleDC(0);

	//on recreer l'entete bitmap d'origine
	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = informations.width;
	bmpinfo.bmiHeader.biHeight = informations.height;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = informations.PixelSize ;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = 0;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = informations.Numcolors ;
	bmpinfo.bmiHeader.biClrImportant = informations.Numcolors;
	
	//on creer le bitmap
	*hbmp = CreateDIBSection(0, &bmpinfo, DIB_PAL_COLORS, &pBits, NULL, 0);

	//on selectionne le bitmap dans notre contexte de peripherique
	SelectObject(hdcMem,*hbmp);

	return hdcMem;

}

void AddEcranToBitmap(HBITMAP hbmp, InfoBitmap informations , void * buffer)
{
	//on recopie les bits dans le bitmap du contexte de peripherique
	SetBitmapBits(hbmp,informations.UncompressedSize,buffer);
}