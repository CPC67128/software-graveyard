package photoshed.formats;

import java.io.File;
import java.io.FileInputStream;

import javax.swing.ImageIcon;

import java.awt.image.BufferedImage;
import java.awt.Image;
import java.awt.Toolkit;
import java.awt.Graphics2D;
import java.awt.Canvas;
import java.awt.image.ColorConvertOp;

import photoshed.formats.ExtensionChecker;
import photoshed.formats.ImageIO;
import photoshed.formats.ImageReaderWriter;

/**
 *
 *
 *
 */

public final class GIF extends ExtensionChecker implements ImageIO
{
	/**
	 *
	 *
	 *
	 */

	public GIF()
	{
		extension.put("GIF", this);
		description = "GIF Image Files";
		version = "v 0.2";
	}

	/**
	 *
	 *
	 *
	 */

	public BufferedImage read(File f, boolean couleur) {
		return ImageReaderWriter.read(f, couleur);
	}

	/**
	 *
	 *
	 *
	 */

	public void write(BufferedImage imageSource, File f) {
		ImageReaderWriter.write(imageSource, f, "gif");
	}
}