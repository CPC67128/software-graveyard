/***
 *** Projet WRC - Serveur WRC
 ***
 *** Aurélien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (école d'ingénieurs 3IL - promotion 2004)
 ***
 *** Fichier : prototypes.h
 *** Description : prototypes des fonctions
 ***
 ***/

/***{ Prototypes : debug.cpp }***/

bool InitDebugFile ();
bool AddInfoDebugFile (char * information);
bool AddErrDebugFile (char * erreur);
bool CloseDebugFile ();
void GetDate ();

/***{ Prototypes : envoi_ecran.cpp }***/

void BoucleEnvoiImage(int insock);

/***{ Prototypes : exportation_image.cpp }***/

void * InitBitmapAndMemory(InfoBitmap * informations, HDC * hdcDest , HBITMAP * hbmp);
void PutBitmapInMemory(InfoBitmap informations, HDC hdcDest , HBITMAP hbmp , void * receive );

/***{ Prototypes : jpeg.cpp }***/

void jpeg_decompress(unsigned char *dst, unsigned char *src, int size, int *w, int *h);
int jpeg_compress(char *dst, char *src, int width, int height, int dstsize, int quality);

/***{ Prototypes : registre.cpp }***/

bool EnregistrerPassword (char * szPassword);
bool LirePassword (char ** buffer, DWORD taille_buffer);
bool SupprimerPassword ();
char * SupprimerArrierePlan (int * ad);
void MettreArrierePlan (char * fichier, int ad);

/***{ Prototypes : rsa.cpp }***/

bool CreerCles ();
unsigned __int64 Crypter (char * chaine, int tailleChaine);
unsigned __int64 Decrypter (unsigned __int64 nombreCrypte);
unsigned __int64 Modulo (unsigned __int64 x, unsigned __int64 y);
bool EstPremier (unsigned __int64 nombre);
unsigned __int64 PGCD (unsigned __int64 a, unsigned __int64 b);
unsigned __int64 EuclideEtendu (unsigned __int64 b, unsigned __int64 n);
unsigned __int64 PuissanceModulaire(unsigned __int64 a, unsigned __int64 x, unsigned __int64 n);
bool CompleterRSAInfoServeur (INFOSERVEUR * infoServeur);
bool RenseignerSomme (char * chaine, int tailleChaine);

/***{ Prototypes : serveur.cpp }***/

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow);
BOOL CALLBACK ServerMainDialogProc (HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

/***{ Prototypes : sha.cpp }***/

void padding (char * szPassword);
unsigned long decalage_circulaire (unsigned long x, unsigned long n, unsigned long masque);
void sha ();
void boucle_sha (unsigned char * bloc);
char * algorithme_sha (char * szPassword);
bool ComparerPassword (unsigned __int64 messageCrypte);

/***{ Prototypes : socket_serveur.cpp }***/

SOCKET CreateServerSocket (HWND hWnd);
BOOL CloseServerSocket (SOCKET socket_serveur);

/***{ Prototypes : systray.cpp }***/

BOOL InitSysTrayIcon (HWND hWnd, HICON hIcon, char * tooltip);
BOOL AddIcon ();
BOOL RemoveIcon ();
BOOL ChangeIcon (HICON hIcon);
BOOL ChangeTooltip (char * tooltip);
DWORD ShowMenu (HMENU hMenuPopup);

/***{ Prototypes : thread_ecran.cpp }***/

DWORD WINAPI DemarrerThreadEcran (LPVOID lpParam);

/***{ Prototypes : thread_clavier_souris.cpp }***/

DWORD WINAPI DemarrerThreadClavierSouris (LPVOID lpParam);

/***{ Prototypes : thread_fichier.cpp }***/

DWORD WINAPI DemarrerThreadFichier (LPVOID lpParam);
