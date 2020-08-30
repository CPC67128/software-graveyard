package photoshed.filtres.watershed;

import java.awt.image.BufferedImage;
import java.awt.Point;

public final class WSStat {

	private static final int RM = 3092271;

	private static final int RD = 20;

	protected WSRegion [] regs;

	private int max;

	private BufferedImage src;

	public WSStat(BufferedImage src, int nbRegions) {
		regs = new WSRegion[nbRegions];
		max = 0;
		this.src = src;
	}

	public boolean contient(WSPoint p) {
		if (rechercherWSPointA(p.x, p.y) == null)
			return false;
		return true;
	}

	public WSPoint rechercherWSPointA(int x, int y) {
		WSPoint ret = null;

		for (int i = max - 1; i >= 0; i--)
			if ((ret = regs[i].rechercherWSPointA(x, y)) != null)
				return ret;
		return null;
	}

	public WSPoint rechercherWSPointA(short x, short y) {
		WSPoint ret = null;

		for (int i = max - 1; i >= 0; i--)
			if ((ret = regs[i].rechercherWSPointA(x, y)) != null)
				return ret;
		return null;
	}

	public WSRegion rechercherWSRegion(int label) {
		int lbl = ((label - RM) / RD) - 1;
		if (lbl < 0 || lbl >= regs.length)
			return null;
		return regs[lbl];
	}

	public void ajouter(WSPoint p, int label) {
		int lbl = ((label - RM) / RD) - 1;
		if (regs[lbl] == null) {
			regs[lbl] = new WSRegion(label);
			regs[lbl].ajouter(p);
			max++;
		}
		else
			regs[lbl].ajouter(p);
	}

	public BufferedImage getSource() {
		return src;
	}

	public int nbRegions() {
		return max;
	}

	public WSRegion[] getRegions() {
		return regs;
	}

	public void clear() {
		src = null;
		for (int i = max - 1; i >= 0; i--)
			if (regs[i] != null) {
				regs[i].clear();
				regs[i] = null;
			}
		regs = null;
	}

	public String toString() {
		StringBuffer sb = new StringBuffer((max == 0)?
						("Il n'y a aucune région dans l'image\n"):
						("L'image contient " + max + " région" + 
						((max == 1)?(""):("s")) + "\n\nCouleurs à l'écran son forme RGB\nAires en pixels\nPourcentages par rapport à l'image entiére\n"));
		int a = 0,
			j = 0;
		Point p = null;
		String t;
		sb.append("Les coordonnées X et Y correspondent aux centre des régions\n'Niveau' et le niveau de gris moyen de la zone\n\n");
		sb.append("Région\tCouleur\tAire\t\t  %  \t\t X  \t  Y  \tNiveau\n");
		for (int i = 0; i < max; i++) {
			sb.append(i + 1).append("\t\t").append(Integer.toHexString(regs[i].label)).append("\t");
			a = regs[i].aire();
			sb.append(a).append("\t\t");
			t = "" + (a * 100.0F) / (src.getWidth() * src.getHeight());
			if (t.length() > 7)
				t = t.substring(0, 7);
			sb.append(t);
			for (j = 7 - t.length(); j > 0 ;j--)
				sb.append(" ");
			sb.append("\t");
			p = regs[i].centre();
			sb.append(p.x).append("\t").append(p.y);
			sb.append("\t").append(regs[i].moyenneDeCouleur()).append("\n");
		}

		return sb.toString();
	}
}