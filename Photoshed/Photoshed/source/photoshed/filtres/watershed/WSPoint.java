package photoshed.filtres.watershed;

public class WSPoint {

	public short x;

	public short y;

	byte couleur;

	public WSPoint(int x, int y, int couleur) {
		this.x = (short) x;
		this.y = (short) y;
		this.couleur = (byte)(couleur & 0xff);
	}

	public WSPoint(short x, short y, int couleur) {
		this.x = x;
		this.y = y;
		this.couleur = (byte)(couleur & 0xff);
	}

	public boolean estIdentique(int x, int y) {
		return this.x == (short)x && this.y == (short) y;
	}

	public boolean estIdentique(short x, short y) {
		return this.x == x && this.y ==  y;
	}
}