/***
 *** Projet WRC - Serveur WRC
 ***
 *** Aurélien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (école d'ingénieurs 3IL - promotion 2004)
 ***
 *** Fichier : sha.cpp
 *** Description : implémentation de l'algorithme SHA
 ***
 ***/

#include "include.h"

/***{ Variables globales }***/

unsigned long A;
unsigned long B;
unsigned long C;
unsigned long D;
unsigned long E;
unsigned char ** blocs;
char messageDigest [41];
int nbBlocs;

/***[ padding ]***/

void padding (char * szPassword)
{
	// Déclarations des variables
	unsigned char bloc [64];
	unsigned char c;
	__int64 taille_fichier;
	long taille_fichier_temp;

	// Initialisation
	blocs = (unsigned char **) malloc(0);
	nbBlocs = 0;
	taille_fichier = 0;
	taille_fichier_temp = 0;

	// Traitement de la chaîne passée en paramêtres
	while (szPassword [taille_fichier] != '\0')
	{
		c = szPassword [taille_fichier];
		bloc [taille_fichier % 64] = c;
		taille_fichier++;
		taille_fichier_temp++;
		if (taille_fichier % 64 == 0)
		{
			blocs = (unsigned char **) realloc(blocs, sizeof(unsigned char *) * ((int) taille_fichier / 64));
			nbBlocs ++;
			blocs [((int) taille_fichier / 64) - 1] = (unsigned char *) malloc (sizeof(unsigned char) * 64);
			memcpy (blocs [((int) taille_fichier / 64) - 1], bloc, 64);
		}
	}

	bloc [taille_fichier % 64] = 128;
	taille_fichier++;

	if (taille_fichier % 64 == 0)
	{
		blocs = (unsigned char **) realloc(blocs, sizeof(unsigned char *) * ((int) taille_fichier / 64));
		nbBlocs ++;
		blocs [((int) taille_fichier / 64) - 1] = (unsigned char *) malloc (sizeof(unsigned char) * 64);
		memcpy (blocs [((int) taille_fichier / 64) - 1], bloc, 64);
	}

	if (taille_fichier % 64 > 56)
	{
		while(taille_fichier % 64 !=0)
			bloc [taille_fichier++ % 64] = 0;
		blocs = (unsigned char **) realloc(blocs, sizeof(unsigned char *) * ((int) taille_fichier / 64));
		nbBlocs ++;
		blocs [((int) taille_fichier / 64) - 1] = (unsigned char *) malloc (sizeof(unsigned char) * 64);
		memcpy (blocs [((int) taille_fichier / 64) - 1], bloc, 64);
	}

	// Il faut réserver normalement 64 bits pour la longueur, mais le plus grand entier en C
	// est codé sur 32 bits, donc on complète les 32 bits précédents par des 0 et on y ajoutera
	// les 32 bits de notre entier contenant la longueur du message.
	while(taille_fichier % 64 < 60) 	
		bloc [taille_fichier++ % 64] = 0;

	// Ajout de la représentation binaire de la longueur du message initial
	taille_fichier_temp *= 8;

	bloc [taille_fichier++ % 64] = taille_fichier_temp / 16777216;
	taille_fichier_temp = taille_fichier_temp % 16777216;

	bloc [taille_fichier++ % 64] = taille_fichier_temp / 65536;
	taille_fichier_temp = taille_fichier_temp % 65536;

	bloc [taille_fichier++ % 64] = taille_fichier_temp / 256;
	taille_fichier_temp = taille_fichier_temp % 256;

	bloc [taille_fichier++ % 64] = taille_fichier_temp / 1;

	blocs = (unsigned char **) realloc(blocs, sizeof(unsigned char *) * ((int) taille_fichier / 64));
	nbBlocs ++;
	blocs [((int) taille_fichier / 64) - 1] = (unsigned char *) malloc (sizeof(unsigned char) * 64);
	memcpy (blocs [((int) taille_fichier / 64) - 1], bloc, 64);
}

/***[ decalage_circulaire ]***/

unsigned long decalage_circulaire (unsigned long x, unsigned long n, unsigned long masque)
{
	unsigned long resultat;
	unsigned long temp_decalage = x & masque;
	resultat = x << n;
	resultat = resultat + (temp_decalage >> (32 - n));
	return resultat;
}

/***[ sha ]***/

void sha ()
{
	// Variables
	int i;

	// Initialisation des variables
	A = 0x67452301;
	B = 0xEFCDAB89;
	C = 0x98BADCFE;
	D = 0x10325476;
	E = 0xC3D2E1F0;

	for (i = 0; i < nbBlocs; i++)
		boucle_sha(blocs[i]);
}

/***[ boucle_sha ]***/

void boucle_sha (unsigned char * bloc)
{
	// Variables
	unsigned int i;
	unsigned long W [80];
	unsigned long M [16];
	unsigned long temp;

	unsigned long oldA = A;
	unsigned long oldB = B;
	unsigned long oldC = C;
	unsigned long oldD = D;
	unsigned long oldE = E;

	// Initialisation des variables
	for (i = 0; i < 64; i += 4)
		M[((int) i / 4)] = bloc[i] * 16777216 + bloc[i + 1] * 65536 + bloc[i + 2] * 256 + bloc [i + 3];

	for (i = 0; i < 16; i++)
		W[i] = M[i];

	for (i = 16; i < 80; i++)
		W[i] = decalage_circulaire((W[i-3] ^ W[i-8] ^ W[i-14] ^ W[i-16]), 1, 0x80000000);
	
	// Boucle principale
	for (i = 0; i < 80; i++)
	{
		if (i < 20)
			temp = decalage_circulaire (A, 5, 0xF8000000) + ((B & C) | ((~B) & D)) + E + W[i] + 0x5A827999;
		else if (i < 40)
			temp = decalage_circulaire (A, 5, 0xF8000000) + (B ^ C ^ D) + E + W[i] + 0x6ED9EBA1;
		else if (i < 60)
			temp = decalage_circulaire (A, 5, 0xF8000000) + ((B & C) | (B & D) | (C & D)) + E + W[i] + 0x8F1BBCDC;
		else
			temp = decalage_circulaire (A, 5, 0xF8000000) + (B ^ C ^ D) + E + W[i] + 0xCA62C1D6;
		E = D;
		D = C;
		C = decalage_circulaire (B, 30, 0xFFFFFFFC);
		B = A;
		A = temp;
	}

	// Affichage des valeurs de Hash intermédiaire
	A = A + oldA;
	B = B + oldB;
	C = C + oldC;
	D = D + oldD;
	E = E + oldE;
}

/***[ algorithme_sha ]***/

char * algorithme_sha (char * szPassword)
{
	// Déroulement de l'algorithme
	padding (szPassword);
	sha ();
	sprintf (messageDigest, "%8X%8X%8X%8X%8X", A, B, C, D, E);

	// Renvoi du résultat
	return messageDigest;
}
