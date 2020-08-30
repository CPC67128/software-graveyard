package photoshed.formats;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.FileNotFoundException;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
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

public final class RAW extends ExtensionChecker implements ImageIO, ActionListener
{
	/**
	 *
	 *
	 *
	 */

	public RAW() {
		extension.put("RAW", this);
		description = "RAW Image Files";
		version = "v 0.0 alpha";
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
		ImageReaderWriter.write(imageSource, f, "raw");
	}

	public void actionPerformed(ActionEvent e) {
	}
}