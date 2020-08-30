/*---------------------------------------------------*
 |      FPrecendent.java                             |
 |---------------------------------------------------|
 | Ce fichier définit le filtre FPrecendent qui      |
 | après une fusion, de revenir en arrière dans la   |
 | hiérarchie.                                       |
 *---------------------------------------------------*/

// Désignation du package d'appartenance
package photoshed.filtres;

// Importations des librairies JAVA nécéssaires
import java.awt.image.BufferedImage;
import java.awt.Component;
import javax.swing.JFrame;
import photoshed.fenetres.Picture;
import photoshed.filtres.Filter;
import photoshed.filtres.fusion.FInfos;
import photoshed.filtres.watershed.WS;

// Début de la classe FPrecedent
public final class FPrecedent implements Filter
{
	// ----- Déclaration des attributs de Fusion
	private JFrame fenetre;
	private boolean firstTime = true;

	// ----- Méthodes d'instances

	// Fonction indiquant au programme principal si l'objet que l'on veut traiter peut l'être
	public boolean isApplicable(Picture pic)
	{
		if (pic.property instanceof FInfos)
			if (((FInfos) pic.property).precedent != null)
				return true;
		return false;
	}

	// Fonction qui renvera un objet Picture déjà existant qui représente l'image précédente dans la hiérarchie de fusion
	public Picture execute(Picture pic)
	{
		// Récupération de la JFrame principal de l'application
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

	// Fonction qui ne sert à rien
	public BufferedImage traitement(BufferedImage src, BufferedImage dst)
	{
		return null;
	}

	// Fonction toString de la Classe
	public String toString()
	{
		return "Retour en arrière";
	}
}
// Fin de la classe FPrecedent