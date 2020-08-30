package photoshed.formats;

import java.io.File;

import java.awt.image.BufferedImage;

/**
 *
 *
 *
 */

public interface ImageIO
{

	/**
	 *
	 *
	 *
	 */

	public BufferedImage read(File f, boolean ouvrirEnCouleur);

	/**
	 *
	 *
	 *
	 */

	public void write(BufferedImage imageSource, File f);

	/**
	 *
	 *
	 *
	 */

	public String toString();

} // fin interface FileFormat