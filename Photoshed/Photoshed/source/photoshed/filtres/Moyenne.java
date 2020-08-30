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

public final class Moyenne implements Filter {

	/**
	 *
	 *
	 *
	 */

	public Picture execute(Picture pic){
		BufferedImage src = pic.getSource();
		BufferedImage dst = new BufferedImage(src.getWidth(), src.getHeight(), src.getType());
		Picture retPic = new Picture(pic.getName(), traitement(src, dst));
		retPic.property = pic.property;
		return retPic;
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
		int [] moy = new int[9];
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
					moy[0] = tab[i-1][j-1];
					moy[1] = tab[i][j-1];
					moy[2] = tab[i+1][j-1];
					moy[3] = tab[i+1][j];
					moy[4] = tab[i+1][j+1];
					moy[5] = tab[i][j+1];
					moy[6] = tab[i-1][j+1];
					moy[7] = tab[i-1][j];
					moy[8] = tab[i][j];
					source[k] = moyenne(moy);
				}
				else {
					source[k] = tab[i][j];
				}
				
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
		return "Moyenne";
	}

	public static int moyenne(int[] v)
	{
		int tmp = 0, j = 0;
		for (j = 0; j < v.length; j++)
			tmp += v[j];

		return tmp/v.length;
	}

}