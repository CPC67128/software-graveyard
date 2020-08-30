/*---------------------------------------------------*
 |      FRegion.java                                 |
 |---------------------------------------------------|
 | Ce fichier définit les régions de l'image qui     |
 | constitueront la fusion.                          |
 *---------------------------------------------------*/

// Désignation du package d'appartenance
package photoshed.filtres.fusion;

// Importations des librairies JAVA nécéssaires
import java.util.Vector;
import java.util.Enumeration;

// Début de la classe FRegion
public class FRegion
{
	// ----- Déclaration des attributs de FRegion
	public Integer label;
	public double moyenne;
	public int aire;
	public Vector composantes = new Vector(10, 5);
	public Vector voisins;

	// ----- Constructeurs
	public FRegion(Integer label)
	{
		this.label = label;
		this.moyenne = 0;
		this.aire = 0;
		this.voisins = new Vector(10, 5);
	}

	public FRegion(FRegion fr)
	{
		this.label = new Integer(fr.label.intValue());
		this.moyenne = fr.moyenne;
		this.aire = fr.aire;
		this.composantes = new Vector(10, 5);
		this.voisins = (Vector) fr.voisins.clone();
	}

	// ----- Méthodes d'instances

	// Procédure permettant d'ajouter un nouveau point à la FRegion
	public void ajouterPoint(int couleur)
	{
		this.moyenne += couleur;
		this.aire++;
	}

	// Procédure permettant d'ajouter un voisin
	public void ajouterVoisin(FRegion fr)
	{
		if (!(voisins.contains(fr)) && this != fr)
			voisins.add((Object) fr);
	}

	// Procédure permettant de finaliser notre FRegion après sa construction
	public void finaliserConstruction()
	{
		this.moyenne = this.moyenne / this.aire;
		trierVoisins();
	}

	// Procédure permettant d'ajouter une composante et de sauvegarder l'ensemble de ses données
	public void ajouterComposante(FRegion fr)
	{
		// Appropriation de l'ensemble des voisins viables de cette région
		Enumeration e;
		FRegion temp;
		for (e = fr.voisins.elements(); e.hasMoreElements();)
		{
			temp = (FRegion) e.nextElement();
			this.ajouterVoisin(temp);
			temp.remplacerVoisin(fr, this);
		}
		fr.voisins = null;
		trierVoisins();

		// Appropriation de la liste des régions composantes
		for (e = fr.composantes.elements(); e.hasMoreElements();)
			composantes.add(e.nextElement());
		fr.composantes = null;

		// Appropriation des données de la région
		composantes.add((Object) fr.label);
		this.moyenne = (this.moyenne * this.aire) + (fr.moyenne * fr.aire);
		this.aire += fr.aire;
		this.moyenne = this.moyenne / this.aire;
	}

	// Procédure permettant de remplacer un voisin par un autre voisin
	public void remplacerVoisin(FRegion ancienne, FRegion nouvelle)
	{
		int index_ancienne = voisins.indexOf((Object) ancienne);
		if (index_ancienne >= 0)
			voisins.remove(index_ancienne);
		if (!(voisins.contains(nouvelle)) && this != nouvelle)
		{
			voisins.add((Object) nouvelle);
			trierVoisins();
		}
	}

	// Procédure permettant de trier les voisins selon les moyennes pas ordre croissant, et selon l'aire par ordre croissante
	public void trierVoisins()
	{
		// Tri de la structure
		FRegion max, current;
		int indice, fin = voisins.size() - 1, index;

		while (fin >= 1)
		{
			max = (FRegion) voisins.elementAt(0);
			index = 0;

			for (indice = 1; indice <= fin; indice++)
			{
				current = (FRegion) voisins.elementAt(indice);

				if (current.moyenne > max.moyenne || ((current.moyenne == max.moyenne) && (current.aire > max.aire)))
				{
					max = current;
					index = indice;
				}
			}
			current = (FRegion) voisins.remove(fin);
			voisins.insertElementAt(max, fin);
			voisins.set(index, current);
			fin--;
		}
	}
}