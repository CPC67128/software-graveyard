package photoshed.utils;

import java.security.InvalidParameterException;

public class Pile {

	private Maillon cur;

	private int nbVal;
	
	public Pile() {
		cur = null;
		nbVal = 0;
	}
	
	public void push(Object o) {
		Maillon tmp = new Maillon(o, cur);
		cur = tmp;
		nbVal++;
	}
	
	public Object pop() {
		Object ret = null;
		Maillon tmp = cur;
		try {
			ret = cur.getVal();
			cur = cur.suiv;
			tmp.destroye();
			tmp = null;
			nbVal--;
		}
		catch(NullPointerException e) {}
		return ret;
	}
	
	public boolean isEmpty() {
		return nbVal == 0;
	}
	
	public int size() {
		return nbVal;
	}
	
	public void clear() {
		while(nbVal > 0) {
			pop();
		}
	}
	
	public String toString() {
		String s = "" + this.addresse();
		Maillon m;
		if (isEmpty())
			s += " est vide";
		else {
			s += " {";
			m = cur;
			for(int i = 0; i < nbVal; i++) {
				s += m.toString();
				if ( i < nbVal - 1)
					s += ", ";
				m = m.suiv;
			}
			s += " }";
		}
		return s;
	}
	
	public String addresse() {
		return super.toString();
	}

	private class Maillon {

		private Object val;

		public Maillon suiv;
	
		public Maillon(Object o, Maillon s) {
			val = o;
			suiv = s;
		}
	
		public Object getVal() {
			return val;
		}
	
		public String toString() {
			return "Courant : " + this.addresse() + ", valeur : " + val + "\nSuivant : " + ((suiv != null)?(suiv.addresse()):("null"));
		}
	
		public String addresse() {
			return super.toString();
		}
	
		public void destroye() {
			val = null;
			suiv = null;
		}
	}
}