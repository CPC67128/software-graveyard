/*---------------------------------------------------*
 |      FWS.java                                     |
 |---------------------------------------------------|
 | Ce fichier d�finit la structure mat�rialisant les |
 | donn�es de la FInfos repr�snetant l'image         |
 | Watershed.                                        |
 *---------------------------------------------------*/

// D�signation du package d'appartenance
package photoshed.filtres.fusion;

// Importations des librairies JAVA n�c�ssaires
import photoshed.filtres.watershed.WS;
import photoshed.filtres.fusion.FInfos;

// D�but de la classe FWS
public class FWS extends WS
{
	public FInfos infos;

	public FWS(WS ws, FInfos infos)
	{
		super(ws.getSource(), ws.getGradient(), ws.getNbRegions());
		this.infos = infos;
	}
}
// Fin de la classe FWS