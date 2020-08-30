/*---------------------------------------------------*
 |      FWS.java                                     |
 |---------------------------------------------------|
 | Ce fichier définit la structure matérialisant les |
 | données de la FInfos représnetant l'image         |
 | Watershed.                                        |
 *---------------------------------------------------*/

// Désignation du package d'appartenance
package photoshed.filtres.fusion;

// Importations des librairies JAVA nécéssaires
import photoshed.filtres.watershed.WS;
import photoshed.filtres.fusion.FInfos;

// Début de la classe FWS
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