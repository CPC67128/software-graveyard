package photoshed.formats;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.FileNotFoundException;

import java.awt.image.BufferedImage;
import java.awt.image.ColorConvertOp;

import photoshed.formats.ExtensionChecker;
import photoshed.formats.ImageIO;
import photoshed.formats.ImageReaderWriter;

/**
 *
 *
 *
 */

public final class BMP extends ExtensionChecker implements ImageIO
{

	/**
	 *
	 *
	 *
	 */

	public BMP() {
		extension.put("BMP", this);
		description = "Bitmap Image Files";
		version = "v 0.3";
	}

	/**
	 *
	 *
	 *
	 */

	public BufferedImage read(File f, boolean couleur) {
		return ImageReaderWriter.read(f, couleur);
	}

	public void write(BufferedImage imageSource, File f) {
		System.out.println(ImageReaderWriter.write(imageSource, f, "bmp"));
	}
}