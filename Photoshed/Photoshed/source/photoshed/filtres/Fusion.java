/*---------------------------------------------------*
 |      Fusion.java                                  |
 |---------------------------------------------------|
 | Ce fichier d�finit le filtre fusion.              |
 *---------------------------------------------------*/

// D�signation du package d'appartenance
package photoshed.filtres;

// Importations des librairies JAVA n�c�ssaires
import java.awt.image.BufferedImage;
import java.awt.Component;
import javax.swing.JFrame;
import photoshed.fenetres.Picture;
import photoshed.filtres.Filter;
import photoshed.filtres.fusion.FDBox;
import photoshed.filtres.fusion.FInfos;
import photoshed.filtres.fusion.FWS;
import photoshed.filtres.watershed.WS;

// D�but de la classe Fusion
public final class Fusion implements Filter
{
	// ----- D�claration des attributs de Fusion
	private JFrame fenetre;
	private boolean firstTime = true;
	private Object property;
	private FInfos infos = null;
	private FInfos fwsInfos = null;

	// ----- M�thodes d'instances

	// Fonction indiquant au programme principal si l'objet que l'on veut traiter peut l'�tre
	public boolean isApplicable(Picture pic)
	{
		if (pic.property instanceof FInfos)
			if (((FInfos) pic.property).getNbDifferentMoyenne() > 1)
				return true;
		return (pic.property instanceof WS || pic.property instanceof FWS);
	}

	// Fonction qui renvera un nouvel objet Picture contenant l'image fusionn�e avec tout ses attributs correctement affect�s
	public Picture execute(Picture pic)
	{
		// R�cup�ration de la JFrame principal de l'application
		if (this.firstTime)
		{
			Component cmp = pic.getDesktopPane();
			while(!(cmp instanceof JFrame))
				cmp = cmp.getParent();
			this.fenetre = (JFrame) cmp;
			this.firstTime = false;
		}

		// Traitement de la fusion
		if (pic == null)
			return null;

		property = pic.property;
		BufferedImage origine = pic.getSource();
		BufferedImage resultat = new BufferedImage(origine.getWidth(), origine.getHeight(), BufferedImage.TYPE_INT_RGB);

		resultat = traitement(origine, resultat);
		if (fwsInfos != null)
		{
			FWS fws = new FWS((WS) property, fwsInfos);
			pic.property = fws;
			fwsInfos = null;
		}
		if (resultat == null)
		{
			property = null;
			return null;
		}

		Picture picture = new Picture(pic.getName(), resultat);
		if (property instanceof FInfos)
			infos.precedent = pic;
		else
			infos.precedent = null;
		picture.property = (Object) this.infos;
		fwsInfos = null;
		infos = null;
		property = null;
		return picture;
	}

	// Fonction qui effectue le traitement de la fusion
	public BufferedImage traitement(BufferedImage src, BufferedImage dst)
	{
		// Cr�ation d'une structure FInfos viable et pr�te � l'emploi pour la fusion
		long t = System.currentTimeMillis();
		if (property instanceof FWS)
		{
			System.out.print("FUSION : Cr�ation de FInfos depuis FWS : ");
			infos = new FInfos(((FWS) property).infos);
		}
		else if (property instanceof FInfos)
		{
			System.out.print("FUSION : Cr�ation de FInfos depuis FInfos : ");
			infos = new FInfos((FInfos) property);
		}
		else
		{
			System.out.print("FUSION : Cr�ation de FInfos depuis WS : ");
			infos = new FInfos(src, ((WS) property).getSource());
			fwsInfos = new FInfos(infos);
		}
		System.out.println("TOTAL : " + (System.currentTimeMillis() - t));

		// Demande des param�tres de la fusion
		FDBox fdb = new FDBox(src.getHeight(), src.getWidth(), infos.getNbDifferentMoyenne(), fenetre);
		int seuil = fdb.getSeuil();
		int nbMinima = fdb.getNbMinima();
		int area = fdb.getArea();
		fdb = null;
		if (seuil < 0)
			return null;

		// Traitement de la fusion
		t = System.currentTimeMillis();
		System.out.print("FUSION : d�roulement de l'algorithme de fusion : ");
		infos.traiter(seuil, nbMinima, area, src, dst);
		System.out.println(System.currentTimeMillis() - t);

		// Renvoi de l'objet final
		return dst;
	}

	// Fonction toString de la Classe
	public String toString()
	{
		return "Nouvelle fusion";
	}
}
// Fin de la classe Fusion