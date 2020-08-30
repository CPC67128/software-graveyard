package photoshed.filtres;

import java.awt.image.BufferedImage;
import java.awt.image.SampleModel;
import java.awt.image.WritableRaster;
import java.awt.image.DataBuffer;

import photoshed.fenetres.Picture;
import photoshed.filtres.Filter;

/**
 *
 *
 *
 */

public final class Sobelien implements Filter {

	/**
	 *
	 *
	 *
	 */

	public Picture execute(Picture pic){
		BufferedImage src = pic.getSource();
		BufferedImage dst = new BufferedImage(src.getWidth(), src.getHeight(), src.getType());
		Picture tmp = new Picture(pic.getName(), traitement(src, dst));
		tmp.property = (Object)pic.getSource();
		return tmp;
	}

	/**
	 *
	 *
	 *
	 */

	public boolean isApplicable(Picture pic){
		return true;
	}

	/**
	 *
	 *
	 *
	 */

	public BufferedImage traitement(BufferedImage src, BufferedImage dst) {
		int w = src.getWidth();
		int h = src.getHeight();
		int i, j, k;
		SampleModel smSRC, smDST;
		WritableRaster wrSRC, wrDST;
		DataBuffer dbSRC, dbDST;
		wrSRC = src.getRaster();
		wrDST = dst.getRaster();
		smSRC = wrSRC.getSampleModel();
		smDST = wrDST.getSampleModel();
		dbSRC = wrSRC.getDataBuffer();
		dbDST = wrDST.getDataBuffer();
		
		int [] source = new int[w * h];
		int [][] tab = new int[h][w];
		int sobel = 0;
		float xi = 0f, yi = 0f;

		for (int b = smSRC.getNumBands() - 1; b >= 0; b--) {
			smSRC.getSamples(0, 0, w, h, b, source, dbSRC);

			for (j = 0, k = 0; j < h; j++) {
				for (i = 0; i < w; i++, k++) {
					tab[j][i] = source[k];
				}
			}

			for (i = 0, k = 0; i < h; i++) {
				for (j = 0; j < w; j++, k++) {
					if (i > 0 && i < h - 1 && j > 0 && j < w - 1) {

						yi = -1 * tab[i-1][j-1] + -2 * tab[i-1][j] + -1 * tab[i-1][j+1] +
							tab[i+1][j-1] + 2 * tab[i+1][j] + tab[i+1][j+1];

						xi = -1 * tab[i-1][j-1] + -2 * tab[i][j-1] + -1 * tab[i+1][j-1] +
							tab[i-1][j+1] + 2 * tab[i][j+1] + tab[i+1][j+1] ;

						sobel = (int)Math.sqrt((xi * xi) + (yi * yi));

						if (sobel > 255) sobel = 255;
						source[k] = sobel;
					}
					else
						source[k] = 255;
				}
			}

			smDST.setSamples(0, 0, w, h, b, source, dbDST);
		}

		return dst;
	}

	/**
	 *
	 *
	 *
	 */

	public String toString() {
		return "Gradient Sobelien";
	}
}
