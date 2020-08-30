/*---------------------------------------------------*
 |      FInfos.java                                  |
 |---------------------------------------------------|
 | Ce fichier définit la structure matérialisant les |
 | données d'une fusion opérée sur une image.        |
 *---------------------------------------------------*/

// Désignation du package d'appartenance
package photoshed.filtres.fusion;

// Importations des librairies JAVA nécéssaires
import java.util.Vector;
import java.util.Enumeration;
import java.awt.image.BufferedImage;
import java.awt.image.DataBuffer;
import java.awt.image.SampleModel;
import java.awt.image.WritableRaster;
import photoshed.fenetres.Picture;
import photoshed.filtres.fusion.FRegion;
import photoshed.filtres.watershed.WS;

// Début de la classe FInfos
public class FInfos
{
	// ----- Déclaration des attributs de FInfos
	public Picture precedent = null;
	public int infos_seuil;
	public int infos_nbMinima;
	public int infos_aire;
	public Vector regions = new Vector(10, 10);
	public Vector oldLabels = new Vector(10, 10);
	public Vector newLabels = null;
	public int [][] wsImage = null;

	// ----- Constructeurs
	public FInfos(BufferedImage image_watershed, BufferedImage image_source)
	{
		int hauteur = image_watershed.getHeight();
		int largeur = image_watershed.getWidth();

		// Transposition des données des BufferedImages en tableaux à 2 dimensions d'entiers, avec construction des FRegions initiales
		long t = System.currentTimeMillis();
		WritableRaster wrSource, wrWatershed;
		SampleModel smSource, smWatershed;
		DataBuffer dbSource, dbWatershed;
		int i, j, k;

		int [] donnees = new int [hauteur * largeur];
		wrSource = image_source.getRaster();
		smSource = wrSource.getSampleModel();
		dbSource = wrSource.getDataBuffer();
		smSource.getSamples(0, 0, largeur, hauteur, 0, donnees, dbSource);
		wrSource = null;
		smSource = null;
		dbSource = null;

		int [] [] watershed = new int [hauteur] [largeur];
		int rouge, vert, bleu;
		int index;
		Integer label;
		FRegion fr;
		wrWatershed = image_watershed.getRaster();
		smWatershed = wrWatershed.getSampleModel();
		dbWatershed = wrWatershed.getDataBuffer();

		for (j = hauteur - 1; j >= 0; j--)
			for (i = largeur - 1; i >= 0; i--)
			{
				bleu = smWatershed.getSample(i, j, 0, dbWatershed);
				vert = smWatershed.getSample(i, j, 1, dbWatershed);
				rouge = smWatershed.getSample(i, j, 2, dbWatershed);
				watershed[j][i] = rouge << 16 | vert << 8 | bleu;
				label = new Integer(watershed[j][i]);

				if ((index = oldLabels.indexOf(label)) < 0)
				{
					fr = new FRegion(label);
					fr.ajouterPoint(donnees[(j * largeur) + i]);
					regions.add((Object) fr);
					oldLabels.add((Object) label);
				}
				else
					((FRegion) regions.elementAt(index)).ajouterPoint(donnees[(j * largeur) + i]);
			}
		donnees = null;
		this.wsImage = watershed;
		System.out.println("\nTemps de la transcription : " + (System.currentTimeMillis() - t));

		// Construction des listes de voisins de chaque FRegion
		t = System.currentTimeMillis();
		int label1, label2;
		FRegion fr1, fr2;
		for (j = hauteur - 1; j >= 0; j--)
			for (i = largeur - 1; i > 0; i--)
			{
				label1 = watershed[j][i];
				label2 = watershed[j][i-1];
				if (label1 != label2)
				{
					fr1 = (FRegion) regions.elementAt(oldLabels.indexOf(new Integer(label1)));
					fr2 = (FRegion) regions.elementAt(oldLabels.indexOf(new Integer(label2)));
					fr1.ajouterVoisin(fr2);
					fr2.ajouterVoisin(fr1);
				}
			}
		for (i = largeur - 1; i >= 0; i--)
			for (j = hauteur - 1; j > 0; j--)
			{
				label1 = watershed[j][i];
				label2 = watershed[j-1][i];
				if (label1 != label2)
				{
					fr1 = (FRegion) regions.elementAt(oldLabels.indexOf(new Integer(label1)));
					fr2 = (FRegion) regions.elementAt(oldLabels.indexOf(new Integer(label2)));
					fr1.ajouterVoisin(fr2);
					fr2.ajouterVoisin(fr1);
				}
			}
		System.out.println("Temps de la lecture des voisinages : " + (System.currentTimeMillis() - t));

		// Finalisation de la structure
		t = System.currentTimeMillis();
		trierRegions();
		System.out.println("Temps du tri des régions : " + (System.currentTimeMillis() - t));
	}

	public FInfos(FInfos infos)
	{
		// Il s'agit ici de dupliquer l'ensemble FInfos + FRegion en un nouvel ensemble, complètement autonome
		FRegion fr;
		Enumeration e;
		for (e = infos.regions.elements(); e.hasMoreElements();)
		{
			fr = (FRegion) e.nextElement();
			regions.add((Object) new FRegion(fr));
			oldLabels.add((Object) ((FRegion) regions.lastElement()).label);
		}

		// Il s'agit aussi de remettre à jour toutes les références
		int indice;
		Integer label;
		for (e = regions.elements(); e.hasMoreElements();)
		{
			fr = (FRegion) e.nextElement();
			for (indice = fr.voisins.size() - 1; indice >= 0; indice--)
			{
				label = ((FRegion) fr.voisins.remove(0)).label;
				fr.voisins.add(regions.elementAt(oldLabels.indexOf(label)));
			}
		}

		// Cela ne sert à rien de trier la nouvelle structure car c'est une copie conforme de l'ancienne, qui était triée
	}

	// ----- Méthodes d'instances

	// Procédure permettant de trier et de finaliser les régions si besoin
	private void trierRegions()
	{
		// Finalisation des structures
		for (Enumeration e = regions.elements(); e.hasMoreElements();)
			((FRegion) e.nextElement()).finaliserConstruction();

		// Tri de la structure
		FRegion max, current;
		int indice, fin = regions.size() - 1, index;

		while (fin >= 1)
		{
			max = (FRegion) regions.elementAt(0);
			index = 0;

			for (indice = 1; indice <= fin; indice++)
			{
				current = (FRegion) regions.elementAt(indice);

				if (current.moyenne > max.moyenne || ((current.moyenne == max.moyenne) && (current.aire > max.aire)))
				{
					max = current;
					index = indice;
				}
			}
			current = (FRegion) regions.remove(fin);
			oldLabels.remove(fin);
			regions.insertElementAt(max, fin);
			oldLabels.insertElementAt(max.label, fin);
			regions.set(index, current);
			oldLabels.set(index, current.label);
			fin--;
		}

		// Tri de chaque voisins
		for (Enumeration e = regions.elements(); e.hasMoreElements();)
			((FRegion) e.nextElement()).trierVoisins();
	}

	// La méthode déroulant l'algorithme de fusion sur l'objet courant
	public void traiter(int seuil, int nbLabels, int aire, BufferedImage src, BufferedImage dst)
	{
		infos_seuil = seuil;
		infos_nbMinima = nbLabels;
		infos_aire = aire;

		// ----- Traitement en fonction du seuil

		// Conception de la liste de FRegions de départ
		Vector depart = new Vector(10, 10);
		int nbDifferentMoyenneWatched, indice, fin = regions.size();
		double moyenne = -1;
		FRegion fr;

		for (indice = 0, nbDifferentMoyenneWatched = 0; indice < fin && nbDifferentMoyenneWatched <= nbLabels; indice++)
		{
			fr = (FRegion) regions.elementAt(indice);
			if (fr.moyenne != moyenne)
			{
				nbDifferentMoyenneWatched++;
				moyenne = fr.moyenne;
			}
			if (nbDifferentMoyenneWatched <= nbLabels)
				depart.add(fr);
		}

		// La liste de départ est déjà triée, déroulons maintenant l'algorithme de la fusion
		FRegion ver, poule, renard;
		FRegion gazelle, boa;
		Vector voisins;
		Vector voisinsDuVoisin;
		double difference;
		int index;

		// Boucle du traitement
		while (depart.size() > 0)
		{
			// On prend le premier élément de la liste de départ
			ver = (FRegion) depart.remove(0);

			// On récupère la moyenne des couleurs ainsi que la liste des voisins
			moyenne = ver.moyenne;
			voisins = (Vector) ver.voisins.clone();

			// Pour chaque voisin, on regarde la région qui lui ressemble le plus
			while (voisins.size() > 0)
			{
				// On prend le premier voisin qui nous tombe sous la main
				poule = (FRegion) voisins.remove(0);

				// On regarde si la différence entre ce voisin et notre région de départ ne dépasse pas le seuil
				if ((difference = Math.abs(poule.moyenne - moyenne)) <= seuil)
				{
					// Le voisin va disparaître... mais on n'est pas sûr du boa par lequel cela va arriver
					gazelle = poule;
					boa = ver;

					// On receuille les voisins de notre gazelle, parmi lesquelles se trouve notre boa si ce n'est pas le ver
					voisinsDuVoisin = poule.voisins;

					// On recherche activement le boa
					for (indice = 0; indice < voisinsDuVoisin.size(); indice++)
					{
						// On prend chacun des éléments de la liste du voisin du voisin
						renard = (FRegion) voisinsDuVoisin.elementAt(indice);

						// On regarde la région qui lui ressemble le plus
						if (Math.abs(renard.moyenne - poule.moyenne) < difference)
						{
							difference = Math.abs(renard.moyenne - poule.moyenne);
							boa = renard;
						}
					}

					// Notre bonne gazelle se fait avaler par notre horrible boa
					boa.ajouterComposante(gazelle);

					// On supprime éventuellement la gazelle de la liste de départ
					if (depart.remove(gazelle) && !depart.contains(boa))
						depart.add(boa);

					// Remise à jour éventuelle de la moyenne
					moyenne = ver.moyenne;

					// Suppression éventuelle de la gazelle dans les voisins et dans les régions de l'objet courant
					if (voisins.remove(gazelle) && !voisins.contains(boa) && boa != ver)
						voisins.add(boa);
					regions.remove(gazelle);
				}
			}
		}

		if (aire >= 0)
		{
			// ----- Traitement en fonction de l'aire minimale

			// Conception de la liste de FRegions de départ
			for (Enumeration e = regions.elements(); e.hasMoreElements();)
			{
				fr = (FRegion) e.nextElement();
				if (fr.aire < aire)
					depart.add(fr);
			}

			// Boucle du traitement
			while (depart.size() > 0)
			{
				// On prend le premier élément de la liste de départ
				boa = gazelle = (FRegion) depart.remove(0);

				// On récupère la moyenne des couleurs ainsi que la liste des voisins
				moyenne = gazelle.moyenne;
				difference = 256;

				// Pour chaque voisin, on regarde la région qui lui ressemble le plus
				indice = 0;
				while (indice < gazelle.voisins.size())
				{
					// On prend le premier voisin qui nous tombe sous la main
					poule = (FRegion) gazelle.voisins.elementAt(indice);

					if (Math.abs(poule.moyenne - moyenne) < difference)
					{
						difference = Math.abs(poule.moyenne - moyenne);
						boa = poule;
					}
					indice++;
				}
				regions.remove(gazelle);
				boa.ajouterComposante(gazelle);
				if (boa.aire < aire && !depart.contains(boa))
					depart.add(boa);
				if (boa.aire >= aire && depart.contains(boa))
					depart.remove(boa);
			}
		}

		// Conception du nouveau BufferedImage
		concevoirNouvelleImage(src, dst);
	}

	// Procédure permettant de remplir le BufferedImage qui sera celui de la nouvelle image fusion
	public void concevoirNouvelleImage(BufferedImage origine, BufferedImage destination)
	{
		// On nécéssite un tableau d'int à 2 dimensions représentant l'image d'origine
		int [] [] image = wsImage;
		int j, i;
		wsImage = null;
		int hauteur = origine.getHeight(), largeur = origine.getWidth();
		WritableRaster wrFusion;
		SampleModel smFusion;
		DataBuffer dbFusion;

		if (image == null)
		{
			// Transposition du BufferedImage de départ en un tableau d'entiers
			image = new int [hauteur] [largeur];
			int rouge, vert, bleu;
			wrFusion = origine.getRaster();
			smFusion = wrFusion.getSampleModel();
			dbFusion = wrFusion.getDataBuffer();

			for (j = hauteur - 1; j >= 0; j--)
				for (i = largeur - 1; i >= 0; i--)
				{
					bleu = smFusion.getSample(i, j, 0, dbFusion);
					vert = smFusion.getSample(i, j, 1, dbFusion);
					rouge = smFusion.getSample(i, j, 2, dbFusion);
					image[j][i] = rouge << 16 | vert << 8 | bleu;
				}
		}

		// Construction des correspondances entre anciens et nouveaux labels
		oldLabels = new Vector(10, 10);
		newLabels = new Vector(10, 10);
		Enumeration e1, e2;
		Integer label;
		FRegion fr;
		int indice;
		for (e1 = regions.elements(); e1.hasMoreElements();)
		{
			fr = (FRegion) e1.nextElement();
			label = fr.label;
			for (e2 = fr.composantes.elements(); e2.hasMoreElements();)
			{
				oldLabels.add(e2.nextElement());
				newLabels.add(label);
			}
			fr.composantes = null;
		}

		// Remplissage du BufferedImage résultant
		wrFusion = destination.getRaster();
		smFusion = wrFusion.getSampleModel();
		dbFusion = wrFusion.getDataBuffer();
		int newLabel;

		for (j = hauteur - 1; j >= 0; j--)
			for (i = largeur - 1; i >= 0; i--)
			{
				label = new Integer(image [j] [i]);
				if (oldLabels.contains(label))
					label = (Integer) newLabels.elementAt(oldLabels.indexOf(label));
				newLabel = label.intValue();
				smFusion.setSample(i, j, 0, newLabel & 0xff, dbFusion);
				smFusion.setSample(i, j, 1, (newLabel & 0xff00) >> 8, dbFusion);
				smFusion.setSample(i, j, 2, (newLabel & 0xff0000) >> 16, dbFusion);
			}

		this.oldLabels = null;
		this.newLabels = null;
	}

	// Fonctions renvoyant le nombre de différentes moyennes au sein de l'ensemble des régions
	public int getNbDifferentMoyenne()
	{
		int nbDifferentMoyenne = 0;
		double moyenne = -1;
		double moyenneTest;
		for (Enumeration e = regions.elements(); e.hasMoreElements();)
		{
			moyenneTest = ((FRegion) e.nextElement()).moyenne;
			if (moyenneTest != moyenne)
			{
				moyenne = moyenneTest;
				nbDifferentMoyenne++;
			}
		}
		return nbDifferentMoyenne;
	}
}