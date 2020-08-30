package photoshed.filtres;

import java.awt.image.BufferedImage;

import photoshed.fenetres.Picture;

/**
 *
 *
 *
 */

public interface Filter
{
	/**
	 *
	 *
	 *
	 */

	public Picture execute(Picture pic);

	/**
	 *
	 *
	 *
	 */

	public boolean isApplicable(Picture pic);

	/**
	 *
	 *
	 *
	 */

	public BufferedImage traitement(BufferedImage src, BufferedImage dst);

	/**
	 *
	 *
	 *
	 */

	public String toString();
}