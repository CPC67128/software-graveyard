/*---------------------------------------------------*
 |      FPrecendent.java                             |
 |---------------------------------------------------|
 | Ce fichier d�finit le filtre FPrecendent qui      |
 | apr�s une fusion, de revenir en arri�re dans la   |
 | hi�rarchie.                                       |
 *---------------------------------------------------*/

// D�signation du package d'appartenance
package photoshed.filtres;

// Importations des librairies JAVA n�c�ssaires
import java.awt.image.BufferedImage;
import java.awt.Component;
import javax.swing.JFrame;
import photoshed.fenetres.Picture;
import photoshed.filtres.Filter;
import photoshed.filtres.fusion.FInfos;
import photoshed.filtres.watershed.WS;

// D�but de la classe FPrecedent
public final class FPrecedent implements Filter
{
	// ----- D�claration des attributs de Fusion
	private JFrame fenetre;
	private boolean firstTime = true;

	// ----- M�thodes d'instances

	// Fonction indiquant au programme principal si l'objet que l'on veut traiter peut l'�tre
	public boolean isApplicable(Picture pic)
	{
		if (pic.property instanceof FInfos)
			if (((FInfos) pic.property).precedent != null)
				return true;
		return false;
	}

	// Fonction qui renvera un objet Picture d�j� existant qui repr�sente l'image pr�c�dente dans la hi�rarchie de fusion
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

		FInfos infos = (FInfos) pic.property;
		try
		{
			infos.precedent.setVisible(true);
			infos.precedent.toFront();
			infos.precedent.setSelected(true);			
		}
		catch (java.beans.PropertyVetoException e)
		{
		}
		return null;
	}

	// Fonction qui ne sert � rien
	public BufferedImage traitement(BufferedImage src, BufferedImage dst)
	{
		return null;
	}

	// Fonction toString de la Classe
	public String toString()
	{
		return "Retour en arri�re";
	}
}
// Fin de la classe FPrecedent