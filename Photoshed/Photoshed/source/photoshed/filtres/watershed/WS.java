package photoshed.filtres.watershed;

import java.awt.image.BufferedImage;

public class WS {
	private BufferedImage source;

	private BufferedImage gradient;

	private int nbRegions;

	public WS(BufferedImage imageSource, BufferedImage gradientDeLaSource, int nombreDeRegions) {
		source = imageSource;
		gradient = gradientDeLaSource;
		nbRegions = nombreDeRegions;
	}

	public BufferedImage getSource() {
		return source;
	}

	public BufferedImage getGradient() {
		return gradient;
	}

	public int getNbRegions() {
		return nbRegions;
	}
}