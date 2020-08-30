/***
 *** Projet WRC - Serveur WRC
 ***
 *** Aurélien JOUHAUD, Pierre LE CLAINCHE, Steve FUCHS (école d'ingénieurs 3IL - promotion 2004)
 ***
 *** Fichier : rsa.cpp
 *** Description : implémentation (simplifiée) de l'algorithme RSA
 ***
 ***/

#include "include.h"

/***{ Variables globales }***/

unsigned __int64 n = 0;
unsigned __int64 d = 0;
unsigned __int64 e = 0;
unsigned __int64 somme = 0;

/***[ CreerCles ]***/

bool CreerCles ()
{
	// Variables locales
	unsigned __int64 p = 0;
	unsigned __int64 q = 0;
	unsigned __int64 phi = 0;
	unsigned __int64 pgcd = 0;

	// Génération de deux nombres premiers aléatoires
	p = rand ();

	while (!EstPremier (p))
		p += 1;

	q = rand ();

	while (!EstPremier (q))
		q += 1;

	// Calcul de n = pq
	n = p * q;

	// Calcul de phi (n) = (p-1)(q-1)
	phi = (p - 1) * (q - 1);

	// Recherche de e et de d
	do
	{
		d = 0;
		e = Modulo(rand (), phi);
		pgcd = PGCD (e, phi);
		if (pgcd == 1)
			d = EuclideEtendu (e, phi);
	}
	while (pgcd != 1 || d == 0);

	return true;
}

/***[ Crypter ]***/

unsigned __int64 Crypter (char * chaine, int tailleChaine)
{
	// Variables locales
	unsigned __int64 cryptage = 0;
	unsigned char caractere;
	int indice;

	// Somme de la valeur des lettres de la chaîne
	for (indice = 0, somme = 0; indice <= tailleChaine; indice++)
	{
		caractere = (unsigned char) chaine [indice];
		somme += caractere;
	}

	// Elevation à la puissance
	cryptage = PuissanceModulaire (somme, e, n);

	return cryptage;
}

/***[ Decrypter ]***/

unsigned __int64 Decrypter (unsigned __int64 nombreCrypte)
{
	// Variables locales
	unsigned __int64 nombre = 0;

	// Elevation à la puissance
	nombre = PuissanceModulaire(nombreCrypte, d, n);

	return nombre;
}

/***[ Modulo ]***/

unsigned __int64 Modulo (unsigned __int64 x, unsigned __int64 y)
{
	unsigned __int64 quotient;
	unsigned __int64 reste;

	quotient = x / y;
	reste = x - y * quotient;

	return reste;
}

/***[ EstPremier ]***/

bool EstPremier (unsigned __int64 nombre)
{
	unsigned __int64 puissance = 2;
	bool estPremier = true;

	while (estPremier && puissance < nombre)
	{
		if (Modulo(nombre, puissance) == 0)
			estPremier = false;
		puissance++;
	}

	return estPremier;
}

/***[ PGCD ]***/

unsigned __int64 PGCD (unsigned __int64 a, unsigned __int64 b)
{
	unsigned __int64 pgcd;

	do
	{
		if (a>b)
		{
			pgcd = a;
			a = a - b;
		}
		else
		{
			pgcd = b;
			b = b - a;
		}
	}
	while((a > 0) && (b > 0));

	return pgcd;
}

/***[ EuclideEtendu ]***/

unsigned __int64 EuclideEtendu (unsigned __int64 b, unsigned __int64 n)
{
	unsigned __int64 n0 = n;
	unsigned __int64 b0 = b;
	unsigned __int64 t0 = 0;
	unsigned __int64 t = 1;
	unsigned __int64 q0;
	__int64 temp;

	q0 = n0 / b0;
	while (!EstPremier (q0))
		q0 = q0 - 1;

	unsigned __int64 r = n0 - q0 * b0;

	while (r > 0)
	{
		temp = t0 - q0 * t;
		if (temp >= 0)
			temp = Modulo (temp, n);
		else
			temp = n - (Modulo((-temp), n));

		t0 = t;
		t = temp;
		n0 = b0;
		b0 = r;

		q0 = n0 / b0;
		while (!EstPremier (q0))
			q0 = q0 - 1;

		r = n0 - q0 * b0;
	}

	if (b0 != 1)
		return 0;
	else
		return t;
}

/***[ PuissanceModulaire ]***/

unsigned __int64 PuissanceModulaire(unsigned __int64 a, unsigned __int64 x, unsigned __int64 n)
{
	unsigned __int64 pmodulaire, r = 1;

	while(x > 0)
	{
		if((x % 2) == 1)
		{
			r = Modulo((r * a), n);
		}
		a = Modulo((a * a), n);
		x = x / 2;
		pmodulaire = r;		
	}

	return pmodulaire;
}

/***[ CompleterRSAInfoServeur ]***/

bool CompleterRSAInfoServeur (INFOSERVEUR * infoServeur)
{
	CreerCles ();

	(*infoServeur).e = e;
	(*infoServeur).n = n;

	return true;
}

/***[ ComparerPassword ]***/

bool ComparerPassword (unsigned __int64 messageCrypte)
{
	unsigned __int64 messageDecrypte = Decrypter (messageCrypte);

	if (messageDecrypte == somme)
		return true;
	else
		return false;
}

/***[ RenseignerSomme ]***/

bool RenseignerSomme (char * chaine, int tailleChaine)
{
	// Variables locales
	unsigned char caractere;
	int indice;

	// Somme de la valeur des lettres de la chaîne
	for (indice = 0, somme = 0; indice <= tailleChaine; indice++)
	{
		caractere = (unsigned char) chaine [indice];
		somme += caractere;
	}

	return true;
}
