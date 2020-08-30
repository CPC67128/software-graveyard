package photoshed.formats;

import javax.swing.filechooser.FileFilter;

import java.io.File;
import java.util.Hashtable;
import java.util.Enumeration;

/**
 *
 *
 *
 */

public class ExtensionChecker extends FileFilter
{

	/**
	 *
	 *
	 *
	 */

	protected String description;

	/**
	 *
	 *
	 *
	 */

	protected String version;

	/**
	 *
	 *
	 *
	 */

	protected Hashtable extension;

	/**
	 *
	 *
	 *
	 */

	protected ExtensionChecker()
	{
		extension = new Hashtable();
		description = null;
	}

	/**
	 *
	 *
	 *
	 */

	public boolean accept(File f)
	{
		if (f != null)
		{
			if (f.isDirectory())
				return true;
			String ext = getExtension(f.getName());
			if (ext != null && extension.get(ext) != null)
				return true;
		}
		return false;
	}

	/**
	 *
	 *
	 *
	 */

	public String getDescription()
	{
		String desc = description == null ? "(" : description + " (";
		Enumeration ext = extension.keys();
		if (ext != null)
		{
			desc += "*." + (String) ext.nextElement();
			while (ext.hasMoreElements())
				desc += ", *." + (String) ext.nextElement();
		}
		desc += ")";
		return desc;
	}

	/**
	 *
	 *
	 *
	 */

	public String getExtension(String src)
	{
		int i = src.lastIndexOf('.');
		if (i > 0 && i < src.length() - 1)
			return src.substring(i + 1).toUpperCase();
		return null;
	}

	/**
	 *
	 *
	 *
	 */

	public String getVersion()
	{
		return version != null ? version : "0";
	}
}