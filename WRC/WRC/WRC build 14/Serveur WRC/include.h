/***
 *** Projet WRC - Serveur WRC
 ***
 *** Aurélien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (école d'ingénieurs 3IL - promotion 2004)
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

#define	XMD_H	// évite la définition de <INT32> car conflit avec <windows.h>
#undef FAR		// évite un autre conflit avec <windows.h>

extern "C"		// pour éviter les problèmes de fonctions externes entre C et C++
{
	#include "jpeg\\jpeglib.h"
}

#pragma comment(lib, "ws2_32.lib")
