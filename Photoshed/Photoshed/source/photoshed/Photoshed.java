package photoshed;

import java.awt.Toolkit;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.Insets;
import java.awt.Color;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.event.MouseListener;

import java.io.File;

import java.util.Properties;

import javax.swing.JDesktopPane;
import javax.swing.JInternalFrame;
import javax.swing.JFileChooser;
import javax.swing.filechooser.FileFilter;
import javax.swing.JFrame;
import javax.swing.JDialog;
import javax.swing.WindowConstants;

import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeEvent;

import photoshed.menus.PhotoshedMenuBar;
import photoshed.fenetres.*;
import photoshed.utils.Langage;
import photoshed.formats.*;
import photoshed.aide.Aide;
import photoshed.formats.ImageReaderWriter;

public final class Photoshed extends WindowAdapter implements PropertyChangeListener {

	public static final String G_TITLE = "Photoshed";

	public static final String G_VERSION = "v1.0.0";

	public static final int G_BUILD = 1209;

	public static final String DEFAULT_LNG = "Francais";

	public static final String DEFAULT_INI = "photoshed.ini";

	public String CURRENT_DIR = ".";

	public Langage lng = null;

	private Runtime run = null;

	private JDesktopPane mdi = null;

	private JFileChooser chooser = null;

	private JFrame appli = null;

	private Aide aide = null;

	public Properties proprietes = null;

	private File fichierDeConfig = null;

	private boolean ouvrirEnCouleur = false;

	public Erreur erreur;

	public Option opt;

	private MouseListener mouse;

	public Photoshed(File config, Properties proprietes, Langage l) {

		boolean showSplash;
		Splash spl = null;

		erreur = new Erreur();

		lng = l;

		if (config == null) {
			fichierDeConfig = new File(DEFAULT_INI);
		}

		this.proprietes = proprietes;

		try {
			javax.swing.UIManager.setLookAndFeel(javax.swing.UIManager.getSystemLookAndFeelClassName());
		}
		catch(Exception e) {
			Erreur.print(e, "");
		}

		showSplash = proprietes.getProperty("appli.showsplash", "true").equals("true");

		appli = new JFrame(G_TITLE +" "+ G_VERSION);
		appli.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);

		if (showSplash) {
			spl = new Splash(appli);
			spl.setText(lng.G_LOAD);
		}

		CURRENT_DIR = System.getProperties().getProperty("user.dir", CURRENT_DIR);

		
		run = Runtime.getRuntime();

		if (showSplash) spl.setText(lng.G_LOAD + " " + lng.G_INTERFACE);

		Container co = appli.getContentPane();
		co.setLayout(new BorderLayout());

		opt = new Option();

		if (showSplash) spl.setText(lng.G_LOAD + " " + lng.G_INTERFACE + " " + lng.G_MENUBAR);

		PhotoshedMenuBar pmb = new PhotoshedMenuBar(this);
		mouse = pmb;
		appli.setJMenuBar(pmb);

		mdi = new JDesktopPane();
		mdi.setBackground(Color.darkGray);
		co.add(mdi, BorderLayout.CENTER);

		if (showSplash) spl.setText(lng.G_LOAD + " " + lng.G_INTERFACE + " " + lng.G_TOOLBOX);

		loadChooser();

		appli.addWindowListener(this);
		appli.pack();

		if (showSplash) spl.setText(lng.G_LOAD + " " + lng.G_INTERFACE + " " + lng.G_READY);

		appli.show();

		if (proprietes.getProperty("window.height") == null) {
			String os = System.getProperties().getProperty("os.name", "Linux");

			Insets bordure = appli.getInsets();

			Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();

			if (os.indexOf("Windows") != -1){
				appli.setLocation(0, 0);
				appli.setSize(screen.width, screen.height);
			}
			else {
				appli.setLocation(bordure.left, bordure.left);
				appli.setSize(screen.width - (bordure.left << 1), screen.height - (bordure.bottom + bordure.top));
			}
		}
		else {
			appli.setLocation(Integer.parseInt(proprietes.getProperty("window.x")), 
								Integer.parseInt(proprietes.getProperty("window.y")));
			appli.setSize(Integer.parseInt(proprietes.getProperty("window.width")),
							Integer.parseInt(proprietes.getProperty("window.height")));
		}

		if (showSplash) spl.setText(lng.G_READY);

		if (showSplash) spl.dispose();

		appli.toFront();
		appli.validate();
	}

	private Picture createPicture(File f, ImageIO io) {
		Picture ret = null;
		try {
			if (f != null && io != null) {
				int x = 0,
					i = getPictures().length;
				Insets ins;
				ret = new Picture(f.getPath(), io.read(f, proprietes.getProperty("file.open.color", "false").equals("true")));
				ins = ret.getInsets();
				ins.top *= 6;
				i %= (appli.getHeight() / ins.top);
				x = ins.top * i;
				mdi.add(ret);
				ret.setLocation(x, x);
				ret.addPropertyChangeListener(this);
				setPicture(ret);
				ret.addPopupMenu(mouse);
			}
		}
		catch (OutOfMemoryError mem) {
			if (ret != null)
				ret.destroye();
			erreur.show(appli, mem, lng.E_NOMEM, mem.toString(), lng.DB_OK);
		}
		catch (Exception e) {
			erreur.show(appli, e, lng.E_NOMEM, e.toString(), lng.DB_OK);
		}
		return ret;
	}

	public Picture createPicture(Picture p) {
		try {
			if (p != null) {
				int x = 0,
					i = getPictures().length;
				Insets ins = p.getInsets();
				ins.top *= 6;
				i %= (appli.getHeight() / ins.top);
				x = ins.top * i;
				if (!proprietes.getProperty("appli.newpic", "true").equals("true"))
					closeCurrent();
				mdi.add(p);
				p.setLocation(x, x);
				p.addPropertyChangeListener(this);
				setPicture(p);
				p.addPopupMenu(mouse);
			}
		}
		catch (OutOfMemoryError mem) {
			p.destroye();
			erreur.show(appli, mem, lng.E_NOMEM, mem.toString(), lng.DB_OK);
		}
		catch (Exception e) {
			erreur.show(appli, e, lng.E_NOTEXCEPTED, e.toString(), lng.DB_OK);
		}
		appli.validate();
		appli.repaint();
		return p;
	}

	public void showContents() {
		try {
			if (aide == null)
				aide = new Aide(this);
			else
				aide.toFront();
		}
		catch(OutOfMemoryError mem) {
			closeHelp();
			erreur.show(appli, mem, lng.E_NOMEM, mem.toString(), lng.DB_OK);
		}
		catch (Exception e) {
			erreur.show(appli, e, lng.E_NOTEXCEPTED, e.toString(), lng.DB_OK);
		}
	}

	public void closeHelp() {
		if (aide != null) {
			aide.dispose();
			aide = null;
		}
		appli.validate();
		appli.repaint();
	}

	public void showAbout() {

	}

	public void showOption() {
		if (opt.showOption(appli, proprietes, lng) != null) {
			((PhotoshedMenuBar)appli.getJMenuBar()).reload();
			saveProperties();
			appli.validate();
			appli.repaint();
		}
	}

	public boolean isPicture() {

		boolean retVal = false;
		JInternalFrame [] jifs = mdi.getAllFrames();

		if (jifs != null)
			retVal = jifs.length > 0;

		return retVal;
	}

	public boolean isPictureSelected() {
		boolean ret = false;
		if(getPicture() != null)
			ret = true;
		return ret;
	}

	public Picture getPicture() {
		JInternalFrame[] jifs = mdi.getAllFrames();

		if (jifs != null)
			for(int i = 0; i < jifs.length; i++)
				if (jifs[i].isSelected())
					return (Picture)jifs[i];
		return null;
	}

	public void setPicture(Picture p) {
		if (p == null)
			return;
		try
		{
			Picture tmp = getPicture();
			if (tmp != null)
				tmp.setSelected(false);
			p.setSelected(true);
			if (p.isIcon())
				p.setIcon(false);
		}
		catch(java.beans.PropertyVetoException ex) {
			erreur.show(appli, ex, lng.E_CHGWIN, ex.toString(), lng.DB_OK);
		}
	}

	public void setPicture(int index) {
		try
		{
			Picture tmp = getPicture();
			if (tmp != null)
				tmp.setSelected(false);
			Picture [] pics = getPictures();
			if (pics.length > index)
				pics[index].setSelected(true);
			if (pics[index].isIcon())
				pics[index].setIcon(false);
		}
		catch(java.beans.PropertyVetoException ex) {
			erreur.show(appli, ex, lng.E_CHGWIN, ex.toString(), lng.DB_OK);
		}
	}

	public Picture [] getPictures() {
		JInternalFrame [] jifs = mdi.getAllFrames();

		Picture[] retVal;

		if (jifs != null) {
			retVal = new Picture[jifs.length];
			for (int i = 0; i < retVal.length; i++)
				retVal[i] = (Picture) jifs[i];
		}
		else
			retVal = new Picture[0];

		return retVal;
	}
	

	public void showOpenDialog() {
		try {
			chooser.addChoosableFileFilter(chooser.getAcceptAllFileFilter());
			chooser.setDialogTitle(lng.DB_OPEN);
			chooser.rescanCurrentDirectory();
			chooser.revalidate();
			int valide = chooser.showDialog(appli, lng.DB_OK);

			if (valide == JFileChooser.APPROVE_OPTION)
			{
				open(chooser.getSelectedFile());
			}
		}
		catch (Exception e) {
			erreur.show(appli, e, lng.E_READ, e.toString(), lng.DB_OK);
		}
	}

	public void open(File theFile) {
		if (theFile.isDirectory() || !theFile.exists()) {
			erreur.show(appli, null, lng.E_NOTEXCEPTED, lng.E_READ + "\n" + theFile.getPath(), lng.DB_OK);
			return;
		}
		try {
			FileFilter[] ff = chooser.getChoosableFileFilters();
			FileFilter all = chooser.getAcceptAllFileFilter();
			for (int i = 0; i < ff.length; i++)
				if (ff[i] != all)
					if (ff[i].accept(theFile)) {
						createPicture(theFile, (ImageIO)ff[i]);
						addLastOpenFile(theFile);
						return;
					}
		}
		catch (Exception e) {
			erreur.show(appli, e, lng.E_READ, e.toString(), lng.DB_OK);
		}
	}

	private void addLastOpenFile(File theFile) {
		String s = proprietes.getProperty("file.last.nb"),
			name = theFile.getPath();
		int index = -1,
			max;
		if (s == null)
			proprietes.setProperty("file.last.nb", s = "4");
		max = Integer.parseInt(s);
		for (int i = 0; i < max && index < 0; i++) {
			s = proprietes.getProperty("file.last." + i);
			if (s == null)
				index = i;
			else
				if (s.equals(name))
					index = i;
		}
		if (index < 0) {
			for (int i = max - 2; i >= 0; i--)
				proprietes.setProperty("file.last." + (i + 1), proprietes.getProperty("file.last." + i));
			proprietes.setProperty("file.last.0", name);
		}
		else {
			for (int i = index - 1; i >= 0; i--)
				proprietes.setProperty("file.last." + (i + 1), proprietes.getProperty("file.last." + i));
			proprietes.setProperty("file.last.0", name);
		}
	}

	public void saveCurrent() {
		showSaveAsDialog();
	}

	public void showSaveAsDialog() {
		try {
			chooser.setDialogTitle(lng.DB_SAVEAS);
			chooser.removeChoosableFileFilter(chooser.getAcceptAllFileFilter());
			chooser.setFileFilter((chooser.getChoosableFileFilters())[0]);
			int valide = chooser.showDialog(appli, lng.DB_SAVE);

			if (valide == JFileChooser.APPROVE_OPTION)
			{
				File ret = chooser.getSelectedFile();
				System.out.println(ret.getPath());
				if (ret.isDirectory())
					return;
				if (ret.exists()) {
					// demande une confirmation d'effacement de l'ancien fichier
				}
				if (chooser.getFileFilter() == chooser.getAcceptAllFileFilter()) {
					FileFilter[] ff = chooser.getChoosableFileFilters();
					for (int i = 0; i < ff.length; i++) {
						if (ff[i] != chooser.getAcceptAllFileFilter())
							if (ff[i].accept(ret)) {
								((ImageIO)ff[i]).write(getPicture().getSource(), ret);
								addLastOpenFile(ret);
								return;
							}
					}
				}
				else {
					((ImageIO)chooser.getFileFilter()).write(getPicture().getSource(), ret);
				}
			}
		}
		catch (Exception e) {
			erreur.show(appli, e, lng.E_WRITE, e.toString(), lng.DB_OK);
		}
	}

	public void closeCurrent() {
		Picture pic = getPicture();
		if (pic != null)
		{
			pic.destroye();

			Picture[] tmp = getPictures();
			if (tmp.length > 0)
				setPicture(tmp[tmp.length - 1]);

			run.gc();
			run.runFinalization();
		}
	}

	public void closeAll() {
		Picture[] pic = getPictures();

		for (int i = 0; i < pic.length; i++) {
			pic[i].destroye();
			pic[i] = null;
		}

		if (pic.length > 0) {
			pic = null;
			run.gc();
			run.runFinalization();
		}
	}

	public void exit() {
		saveProperties();
		closeAll();
		appli.dispose();
		System.runFinalization();
		System.exit(0);
	}

	public static Properties loadProperties(File fichierDeConfig, Properties proprietes) {
		boolean defaut = false;
		if (proprietes == null) {
			proprietes = new Properties();
		}
		else {
			proprietes.clear();
		}
		if (fichierDeConfig == null) {
			defaut = true;
			fichierDeConfig = new File(DEFAULT_INI);
		}
		try {
			java.io.FileInputStream fis = new java.io.FileInputStream(fichierDeConfig);
			proprietes.load(fis);
			fis.close();
		}
		catch(java.io.IOException e) {
			proprietes.setProperty("appli.language", DEFAULT_LNG);
			proprietes.setProperty("file.open.color", "false");
			proprietes.setProperty("file.last.nb", "4");
			if (!defaut) {
				Erreur.print(e, "");
			}
		}
		return proprietes;
		
	}

	public void saveProperties() {
		if (proprietes == null) {
			return;
		}
		if (fichierDeConfig == null) {
			fichierDeConfig = new File(DEFAULT_INI);
		}
		proprietes.setProperty("appli.language", lng.G_LANGAGE_FILENAME);
		if (proprietes.getProperty("window.save", "true").equals("true")) {
			proprietes.setProperty("window.height", "" + appli.getHeight());
			proprietes.setProperty("window.width", "" + appli.getWidth());
			proprietes.setProperty("window.x", "" + appli.getX());
			proprietes.setProperty("window.y", "" + appli.getY());
		}
		else {
			proprietes.remove("window.height");
			proprietes.remove("window.width");
			proprietes.remove("window.x");
			proprietes.remove("window.y");
		}
		try {
			java.io.FileOutputStream fos = new java.io.FileOutputStream(fichierDeConfig);
			proprietes.store(fos, "");
			fos.close();
		}
		catch(java.io.IOException e) {
			erreur.show(appli, e, lng.E_WRITE, e.toString(), lng.DB_OK);
		}
	}

	public void tile() {
		
	}

	public void cascade() {
		Picture [] pics = getPictures();
		Picture selected = getPicture();

		int x = 0;
		Insets ins = selected.getInsets();
		ins.top *= 6;

		for (int i = 0; i < pics.length; i++) {
			if (pics[i] != selected) {
				pics[i].setLocation(x, x);
				x += ins.top;
				pics[i].toFront();
			}
		}
		selected.setLocation(x, x);
		selected.toFront();
	}

	public void windowClosing(WindowEvent we) {
		exit();
	}

	public void propertyChange(PropertyChangeEvent evt) {
		Container c = (Container)evt.getSource();
		if (c instanceof Picture) {
			String txt = (((Picture)c).getName() == null)?(""):(" [" + ((Picture)c).getName() + "]");
			appli.setTitle(G_TITLE +" "+ G_VERSION + txt);
		}
	}

	private void loadChooser() {
		if (chooser == null)
			chooser = new JFileChooser(CURRENT_DIR);
		else
			chooser.resetChoosableFileFilters();

		chooser.addChoosableFileFilter(new BMP());
		chooser.addChoosableFileFilter(new GIF());
		chooser.addChoosableFileFilter(new JPG());
		chooser.addChoosableFileFilter(new PNG());
		chooser.addChoosableFileFilter(new TIFF());

		chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
		chooser.setMultiSelectionEnabled(false);
		chooser.setFileFilter(chooser.getAcceptAllFileFilter());
	}

	public static void main(String [] argv) {

		Runtime r = Runtime.getRuntime();
		boolean isTraceable = false;
		File fichierConfig = null;
		Langage lng = null;
		Properties p = null;

		if (argv.length > 0)
			for (int i = 0; i < argv.length; i++) {
				if(argv[i].equals("-debug")) {
					System.out.println("Debug Mode");
					isTraceable = true;
					java.util.Properties prop = System.getProperties();
					prop.list(System.out);
				}
				else if (argv[i].equals("-gc"))
					r.gc();
				else if (argv[i].equals("-config") && i < argv.length - 1)
					fichierConfig = new File(argv[++i]);
				else {
					System.err.println("Argument incorrecte : " + argv[i]);
					System.err.println("Usage : Photoshed [-option [<fichier>]]");
					System.err.println("Options : ");
					System.err.println("\t-debug\t\t\t: Active les fonctions de debuggages");
					System.err.println("\t-gc\t\t\t\t: Démarre le \"garbage collector\"");
					System.err.println("\t-config <fichier>\t\t: Utilise un fichier de configuration\n" +
								"\t\t\t\tdifférent de photoshed.ini");
					System.exit(0);
				}
			}

		r.traceInstructions(isTraceable);
		r.traceMethodCalls(isTraceable);

		p = loadProperties(fichierConfig, null);
		lng = new Langage(p.getProperty("appli.language", DEFAULT_LNG));

		try {
			Photoshed w = new Photoshed(fichierConfig, p, lng);
		} catch (Exception ex) {
			Erreur.print(ex, lng.E_LOAD_CLASS + "\n" + ex.getMessage() + "\n" + lng.E_STOP);
			System.exit(1);
		}
	}
}