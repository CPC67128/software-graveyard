/***
 *** Projet WRC - Client WRC
 ***
 *** Aurélien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (école d'ingénieurs 3IL - promotion 2004)
 ***
 *** Fichier : prototypes.h
 *** Description : prototypes des fonctions
 ***
 ***/

/***{ Prototypes : client.cpp }***/

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow);
LRESULT CALLBACK ClientWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/***{ Prototypes : connexion.cpp }***/

BOOL CALLBACK ConnexionDialogProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK PasswordDialogProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool VerifierIP ();
bool ResolutionAdresseIP ();

/***{ Prototypes : debug.cpp }***/

bool InitDebugFile ();
bool AddInfoDebugFile (char * information);
bool AddErrDebugFile (char * erreur);
bool CloseDebugFile ();
void GetDate ();

/***{ Prototypes : importation_image.cpp }***/

HDC InitBitmap(InfoBitmap informations, HBITMAP * hbmp);
void AddEcranToBitmap(HBITMAP hbmp, InfoBitmap informations , void * buffer);

/***{ Prototypes : jpeg.cpp }***/

void jpeg_decompress(unsigned char *dst, unsigned char *src, int size, int *w, int *h);
int jpeg_compress(char *dst, char *src, int width, int height, int dstsize, int quality);

/***{ Prototypes : reception_ecran.cpp }***/

void BoucleReceptionImage (Ecran * informations, int sock);

/***{ Prototypes : registre.cpp }***/

bool EnregistrerDerniereAdresseIP (char * adresse_ip);
bool LireDerniereAdresseIP (char ** buffer, DWORD taille_buffer);
bool LireInfoClient (INFOCLIENT * infoClient);
bool EnregistrerInfoClient (INFOCLIENT * infoClient);

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

/***{ Prototypes : sha.cpp }***/

void padding (char * szPassword);
unsigned long decalage_circulaire (unsigned long x, unsigned long n, unsigned long masque);
void sha ();
void boucle_sha (unsigned char * bloc);
char * algorithme_sha (char * szPassword);

/***{ Prototypes : thread_clavier_souris.cpp }***/

DWORD WINAPI DemarrerThreadClavierSouris (LPVOID lpParam);

/***{ Prototypes : thread_ecran.cpp }***/

DWORD WINAPI DemarrerThreadEcran (LPVOID lpParam);
HDC mis_en_memoire();

/***{ Prototypes : thread_fichier.cpp }***/

DWORD WINAPI DemarrerThreadFichier (LPVOID lpParam);
