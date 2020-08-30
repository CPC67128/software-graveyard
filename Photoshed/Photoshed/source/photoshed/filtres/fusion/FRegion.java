/*---------------------------------------------------*
 |      FRegion.java                                 |
 |---------------------------------------------------|
 | Ce fichier d�finit les r�gions de l'image qui     |
 | constitueront la fusion.                          |
 *---------------------------------------------------*/

// D�signation du package d'appartenance
package photoshed.filtres.fusion;

// Importations des librairies JAVA n�c�ssaires
import java.util.Vector;
import java.util.Enumeration;

// D�but de la classe FRegion
public class FRegion
{
	// ----- D�claration des attributs de FRegion
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

	// ----- M�thodes d'instances

	// Proc�dure permettant d'ajouter un nouveau point � la FRegion
	public void ajouterPoint(int couleur)
	{
		this.moyenne += couleur;
		this.aire++;
	}

	// Proc�dure permettant d'ajouter un voisin
	public void ajouterVoisin(FRegion fr)
	{
		if (!(voisins.contains(fr)) && this != fr)
			voisins.add((Object) fr);
	}

	// Proc�dure permettant de finaliser notre FRegion apr�s sa construction
	public void finaliserConstruction()
	{
		this.moyenne = this.moyenne / this.aire;
		trierVoisins();
	}

	// Proc�dure permettant d'ajouter une composante et de sauvegarder l'ensemble de ses donn�es
	public void ajouterComposante(FRegion fr)
	{
		// Appropriation de l'ensemble des voisins viables de cette r�gion
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

		// Appropriation de la liste des r�gions composantes
		for (e = fr.composantes.elements(); e.hasMoreElements();)
			composantes.add(e.nextElement());
		fr.composantes = null;

		// Appropriation des donn�es de la r�gion
		composantes.add((Object) fr.label);
		this.moyenne = (this.moyenne * this.aire) + (fr.moyenne * fr.aire);
		this.aire += fr.aire;
		this.moyenne = this.moyenne / this.aire;
	}

	// Proc�dure permettant de remplacer un voisin par un autre voisin
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

	// Proc�dure permettant de trier les voisins selon les moyennes pas ordre croissant, et selon l'aire par ordre croissante
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