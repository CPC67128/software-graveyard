/***
 *** Projet WRC - Client WRC
 ***
 *** Aur�lien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (�cole d'ing�nieurs 3IL - promotion 2004)
 ***
 *** Fichier : debug.cpp
 *** Description : fonctions de gestion du fichier de debug
 ***
 ***/

#include "include.h"

/***{ Variables globales }***/

char date_actuelle [TAILLE_DATE];
FILE * debug;

/***[ InitDebugFile ]***/

bool InitDebugFile ()
{
	// V�rification de la valeur du DEBUG_FLAG
	if (!DEBUG_FLAG)
		return false;

	// Ouverture du fichier de debug et initialisation
	debug = fopen (DEBUG_FILE, DEBUG_FILE_MODE);
	return true;
}

/***[ AddInfoDebugFile ]***/

bool AddInfoDebugFile (char * information)
{
	// V�rification de la valeur du DEBUG_FLAG
	if (!DEBUG_FLAG)
		return false;

	// Ajout d'une ligne d'information
	GetDate ();
	fprintf(debug, "%s : %s.\n", date_actuelle, information);
	fflush (debug);
	return true;
}

/***[ AddErrDebugFile ]***/

bool AddErrDebugFile (char * erreur)
{
	// V�rification de la valeur du DEBUG_FLAG
	if (!DEBUG_FLAG)
		return false;

	// Ajout d'une ligne d'information
	GetDate ();
	fprintf(debug, "%s : ERREUR : %s.\n", date_actuelle, erreur);
	fflush (debug);
	return true;
}

/***[ CloseDebugFile ]***/

bool CloseDebugFile ()
{
	// V�rification de la valeur du DEBUG_FLAG
	if (!DEBUG_FLAG)
		return false;

	// Ouverture du fichier de debug et initialisation
	fclose (debug);

	return true;
}

/***[ GetDate ]***/

void GetDate ()
{
	// Initialisation
	date_actuelle [0] = '\0';

	// Cr�ation d'une cha�ne pr�sentant la date actuelle
	time_t _temps;
	time (&_temps);
	struct tm * temps = localtime (&_temps);

	sprintf(date_actuelle, "%02d/%02d/%04d %02d:%02d:%02d", temps->tm_mday, temps->tm_mon + 1, temps->tm_year + 1900, temps->tm_hour, temps->tm_min, temps->tm_sec);
}
