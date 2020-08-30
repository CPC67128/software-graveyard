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

public final class JPG extends ExtensionChecker implements ImageIO
{
	public JPG()
	{
		extension.put("JPG", this);
		extension.put("JPEG", this);
		extension.put("JIF", this);
		description = "JPEG & JFIF Image Files";
		version = "v 0.2";
	}

	/**
	 *
	 *
	 *
	 */

	public BufferedImage read(File f, boolean couleur)
	{
		return ImageReaderWriter.read(f, couleur);
	}

	/**
	 *
	 *
	 *
	 */

	public void write(BufferedImage imageSource, File f) {
		System.out.println(ImageReaderWriter.write(imageSource, f, "jpeg"));
	}
}