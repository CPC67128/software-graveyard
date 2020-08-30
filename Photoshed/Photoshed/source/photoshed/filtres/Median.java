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

public final class Median implements Filter {

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
		int [] median = new int[9];
for (int a = smSRC.getNumBands() - 1; a >= 0; a--) {
		smSRC.getSamples(0, 0, w, h, a, source, dbSRC);

		for (j = 0, k = 0; j < h; j++) {
			for (i = 0; i < w; i++, k++) {
				tab[j][i] = source[k];
			}
		}

		for (i = 0, k = 0; i < h; i++) {
			for (j = 0; j < w; j++, k++) {
				if (i > 0 && i < h - 1 && j > 0 && j < w - 1) {
					median[0] = tab[i-1][j-1];
					median[1] = tab[i][j-1];
					median[2] = tab[i+1][j-1];
					median[3] = tab[i+1][j];
					median[4] = tab[i+1][j+1];
					median[5] = tab[i][j+1];
					median[6] = tab[i-1][j+1];
					median[7] = tab[i-1][j];
					median[8] = tab[i][j];
					bubble(median);
				}
				else {
					median[4] = tab[i][j];
				}
				source[k] = median[4];
			}
		}

		smDST.setSamples(0, 0, w, h, a, source, dbDST);
}
		return dst;
	}

	/**
	 *
	 *
	 *
	 */

	public String toString() {
		return "Médian Moyen";
	}

	public static void bubble(int[] v)
	{
		int tmp, i = 0, j = 0;
		boolean f = false;
		while (!f && i < v.length)
		{
			f = true;
			for (j = 0; j < v.length - 1; j++)
				if (v[j] > v[j+1])
				{
					tmp = v[j];
					v[j] = v[j+1];
					v[j+1] = tmp;
					f = false;
				}
			i++;
		}
	}

}