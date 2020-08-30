package photoshed.filtres.watershed;

import java.awt.Rectangle;

import java.util.Vector;
import java.util.Enumeration;

import java.awt.Point;

public class WSRegion {

	protected Vector reg;

	public int label;

	public WSRegion(int label) {
		reg = new Vector(100, 200);

		this.label = label;
	}

	public void ajouter(WSPoint p) {
		reg.add(p);
	}

	public boolean contient(WSPoint p) {
		if (rechercherWSPointA(p.x, p.y) == null)
			return false;
		else
			return true;
	}

	public WSPoint rechercherWSPointA(int x, int y) {
		WSPoint ret = null;
		for (Enumeration e = reg.elements(); e.hasMoreElements();) {
			ret=(WSPoint) e.nextElement();
			if (ret.estIdentique(x, y))
				return ret;
		}
		return null;
	}

	public WSPoint rechercherWSPointA(short x, short y) {
		WSPoint ret = null;
		for (Enumeration e = reg.elements(); e.hasMoreElements();) {
			ret=(WSPoint) e.nextElement();
			if (ret.estIdentique(x, y))
				return ret;
		}
		return null;
	}

	public int aire() {
		return reg.size();
	}

	public Point centre() {
		int x = 0,
			y = 0,
			taille;
		WSPoint p = null;

		for (Enumeration e = reg.elements(); e.hasMoreElements();) {
			p = (WSPoint) e.nextElement();
			x+= p.x;
			y+= p.y;
		}
		taille = aire();
		return new Point(x/taille, y/taille);
	}

	public int moyenneDeCouleur() {
		int couleur = 0;

		for (Enumeration e = reg.elements(); e.hasMoreElements();)
			couleur += (((WSPoint)e.nextElement()).couleur & 0xff);
		
		return couleur/aire();
	}

	public void clear() {
		reg.clear();
		reg = null;
	}

	public Enumeration getWSPoint() {
		return reg.elements();
	}

	public Rectangle getBounds() {
		int x = 0,
			y = 0,
			width = 0,
			height = 0;
		Enumeration e = reg.elements();
		WSPoint p = (WSPoint)e.nextElement();
		x = p.x;
		y = p.y;
		while (e.hasMoreElements()) {
			p = (WSPoint)e.nextElement();
			if (p.x < x) {
				width += (x - p.x);
				x = p.x;
			}
			else if (p.x > x + width)
				width = p.x - x;

			if (p.y < y) {
				height += (y - p.y);
				y = p.y;
			}
			else if (p.y > y + height)
				height = p.y - y;
		}
		return new Rectangle(x, y, width, height);
	}
}