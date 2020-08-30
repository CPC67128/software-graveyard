/***
 *** Projet WRC - Serveur WRC
 ***
 *** Aur�lien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (�cole d'ing�nieurs 3IL - promotion 2004)
 ***
 *** Fichier : include.h
 *** Description : inclusions
 ***
 ***/

/***{ Inclusions }***/

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include "resource.h"
#include "define.h"
#include "types.h"
#include "prototypes.h"
#include "zlib\GZipHelper.h"
#include <windows.h>
#include <setjmp.h>
#include <shellapi.h>
#include <comdef.h>		 // basic COM defs
#include <objbase.h>	 // more basic COM defs..
#include <wininet.h>	 // NOTE: corrects compilation errors w/IActiveDesktop!!
#include <atlbase.h>	// ATL smart pointers
#include <shlguid.h>	// shell GUIDs
#include <shlobj.h>		// IActiveDesktop

#define	XMD_H	// �vite la d�finition de <INT32> car conflit avec <windows.h>
#undef FAR		// �vite un autre conflit avec <windows.h>

extern "C"		// pour �viter les probl�mes de fonctions externes entre C et C++
{
	#include "jpeg\\jpeglib.h"
}

#pragma comment(lib, "ws2_32.lib")
