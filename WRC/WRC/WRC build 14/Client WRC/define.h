/***
 *** Projet WRC - Client WRC
 ***
 *** Aurélien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (école d'ingénieurs 3IL - promotion 2004)
 ***
 *** Fichier : define.h
 *** Description : constantes
 ***
 ***/

/***{ Codes d'erreurs }***/

#define ERROR_SOCKET		1
#define ERROR_SETSOCKOPT	2
#define ERROR_BIND			3
#define ERROR_LISTEN		4
#define ERROR_CONNECT		5

/***{ Protocole de synchronisation }***/

#define CODE_CONNECTION_REQUESTED				"WRC : CONNECTION REQUESTED"
#define CODE_CONNECTION_ACCEPTED				"WRC : CONNECTION ACCEPTED"
#define CODE_CONNECTION_CONFIRMED				"WRC : CONNECTION CONFIRMED"
#define CODE_CONNECTION_REFUSED					"WRC : CONNECTION REFUSED"
#define CODE_INFOSERVEUR_NOPASSWORD_RECEIVED	"WRC : INFOSERVEUR NOPASSWORD RECEIVED"
#define CODE_INFOSERVEUR_PASSWORD_RECEIVED		"WRC : INFOSERVEUR PASSWORD RECEIVED"

/***{ Ports }***/

#define PORT_SERVEUR		19719
#define PORT_ECRAN			19720
#define PORT_CLAVIER_SOURIS	19721
#define PORT_FICHIER		19722

/***{ Identifiants }***/

#define SYSTRAY_UID		WM_USER

/***{ Signaux }***/

#define MYWM_NOTIFYICON						(WM_USER + 1)
#define WM_SOCKET_NOTIFY					(WM_USER + 2)
#define WM_THREAD_ECRAN						(WM_USER + 3)
#define WM_CLAVIER							(WM_USER + 4)
#define WM_SOURIS							(WM_USER + 5)
#define WM_TERMINER_THREAD_CLAVIER_SOURIS	(WM_USER + 6)
#define WM_TERMINER_THREAD_ECRAN			(WM_USER + 7)
#define WM_FICHIER							(WM_USER + 8)
#define WM_TERMINER_THREAD_FICHIER			(WM_USER + 9)

/***{ Debug }***/

#define DEBUG_FLAG		0
#define DEBUG_FILE		"Client WRC.log"
#define DEBUG_FILE_MODE	"w"
#define TEST_LOCAL		0

/***{ Taille des tableaux }***/

#define TAILLE_DATE		20
#define TAILLE_BUFFER	256

/***{ Diverses autres constantes }***/

#define HSCROLL_PETIT_PAS	20
#define HSCROLL_GRAND_PAS	100
#define VSCROLL_PETIT_PAS	20
#define VSCROLL_GRAND_PAS	100

#define BORDURE_HAUT_BAS		50
#define BORDURE_GAUCHE_DROITE	50

/***{ Transmission des écrans }***/

#define METHODE_ECRAN_ZIP					1
#define METHODE_ECRAN_JPEG					2
#define OPTIONS_METHODE_ECRAN_ZIP_DEFAUT	2
#define OPTIONS_METHODE_ECRAN_JPEG_DEFAUT	7
#define METHODE_ECRAN_DEFAUT				METHODE_ECRAN_JPEG
#define OPTIONS_METHODE_ECRAN_DEFAUT		7
#define SLEEP_LENGTH_DEFAUT					0
#define LIMIT_TO_SYSTRAY_DEFAUT				0

#define MODE_TRANSMISSION_NORMAL			1
#define MODE_TRANSMISSION_LOTS				2
#define MODE_TRANSMISSION_DEFAUT			MODE_TRANSMISSION_NORMAL
#define OPTIONS_MODE_TRANSMISSION_DEFAUT	20

/***{ Etats de la souris }***/

#define SOURIS_UP 1
#define SOURIS_DOWN 0
