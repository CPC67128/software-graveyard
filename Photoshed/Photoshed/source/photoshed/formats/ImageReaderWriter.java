package photoshed.formats;

import java.awt.image.BufferedImage;
import java.awt.image.RenderedImage;
import java.awt.image.ColorConvertOp;

import java.io.File;
import java.io.FileOutputStream;
import java.io.BufferedOutputStream;

import com.sun.media.jai.codec.ImageCodec;
import com.sun.media.jai.codec.ImageDecoder;
import com.sun.media.jai.codec.ImageEncoder;
import com.sun.media.jai.codec.FileSeekableStream;
import com.sun.media.jai.codec.SeekableStream;
import com.sun.media.jai.codec.ImageEncodeParam;
import com.sun.media.jai.codec.BMPEncodeParam;
import com.sun.media.jai.codec.TIFFEncodeParam;
import com.sun.media.jai.codec.JPEGEncodeParam;

public final class ImageReaderWriter {

	public static BufferedImage read(File f, boolean couleur) {
		BufferedImage bitmp = null,
				img = null;
		try {
			SeekableStream stream = new FileSeekableStream(f);
			String[] names = ImageCodec.getDecoderNames(stream);
			ImageDecoder dec = ImageCodec.createImageDecoder(names[0], stream, null);
			RenderedImage im = dec.decodeAsRenderedImage();
			bitmp = new BufferedImage(im.getColorModel(), im.copyData(null), im.getColorModel().isAlphaPremultiplied(), null);
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
		if (!couleur) {
			img = new BufferedImage(bitmp.getWidth(), bitmp.getHeight(), BufferedImage.TYPE_BYTE_GRAY);
			ColorConvertOp cop = new ColorConvertOp(null);
			cop.filter(bitmp, img);
		}
		else {
			if (bitmp.getType() == BufferedImage.TYPE_BYTE_INDEXED) {
				img = new BufferedImage(bitmp.getWidth(), bitmp.getHeight(), BufferedImage.TYPE_3BYTE_BGR);
				ColorConvertOp cop = new ColorConvertOp(null);
				cop.filter(bitmp, img);
			} else
				img = bitmp;
			
		}

		return img;
	}

	public static boolean write(BufferedImage img, File f, String format) {
		String fichier = f.getPath();
		FileOutputStream fos = null;
		BufferedOutputStream bos = null;
		ImageEncodeParam encodeParam;
		ImageEncoder imgEncode;

System.out.println("1");
		try {
			fos = new FileOutputStream(fichier);
			bos = new BufferedOutputStream(fos);
		}
		catch(Exception e) {
			try {
				bos.close();
			}
			catch(Exception ex) {}
			try {
				fos.close();
			}
			catch(Exception ex) {}
			return false;
		}
System.out.println("2");
		if(format.equals("bmp"))
			encodeParam = new BMPEncodeParam();
		else if (format.equals("tiff"))
			encodeParam = new TIFFEncodeParam();
		else if (format.equals("jpeg")) {
			encodeParam = new JPEGEncodeParam();
			((JPEGEncodeParam)encodeParam).setQuality(1.0F);
		}
		else if (format.equals("png"))
			encodeParam = new com.sun.media.jai.codec.PNGEncodeParam.RGB();
		else {System.out.println("pas encore");
			try {
				bos.close();
				fos.close();
			}
			catch (Exception e) {}
			return false;
		}
		
		try {System.out.println("jai " + fichier + " " + format);
			imgEncode = ImageCodec.createImageEncoder(format, bos, encodeParam);
			imgEncode.encode(img);
		}
		catch(Exception e) {System.out.println("pas bien");
			e.printStackTrace();
			try {
				bos.close();
				fos.close();
			}
			catch (Exception ex) {}
			return false;
		}
		try {
			bos.close();
			fos.close();
		}
		catch (Exception e) {}
		return true;
	}
}