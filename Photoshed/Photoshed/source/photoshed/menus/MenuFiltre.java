package photoshed.menus;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.event.MenuListener;
import javax.swing.event.MenuEvent;

import java.util.Hashtable;
import java.util.Enumeration;

import java.io.File;

import photoshed.Photoshed;
import photoshed.fenetres.Picture;
import photoshed.utils.Langage;
import photoshed.filtres.*;
import photoshed.menus.Reloadable;

public final class MenuFiltre extends JMenu implements MenuListener, ActionListener, Reloadable {
	private Photoshed waz;

	private Hashtable ash;

	protected MenuFiltre(Photoshed w) {
		super(w.lng.M_FILTERS);
		waz = w;
		Filter tmp;
		JMenuItem mi;
		JMenu m;

		ash = new Hashtable(10, 4);

		m = new JMenu("Atténuation");
		
		tmp = new Median();
		mi = new JMenuItem(tmp.toString());
		mi.addActionListener(this);
		m.add(mi);
		ash.put(mi, tmp);
		add(m);

		tmp = new MedMax();
		mi = new JMenuItem(tmp.toString());
		mi.addActionListener(this);
		m.add(mi);
		ash.put(mi, tmp);

		tmp = new MedMin();
		mi = new JMenuItem(tmp.toString());
		mi.addActionListener(this);
		m.add(mi);
		ash.put(mi, tmp);

		tmp = new Moyenne();
		mi = new JMenuItem(tmp.toString());
		mi.addActionListener(this);
		m.add(mi);
		ash.put(mi, tmp);

		tmp = new Flou();
		mi = new JMenuItem(tmp.toString());
		mi.addActionListener(this);
		m.add(mi);
		ash.put(mi, tmp);

		m = new JMenu("Taille de l'image");
		
		tmp = new Crop();
		mi = new JMenuItem(tmp.toString());
		mi.addActionListener(this);
		m.add(mi);
		ash.put(mi, tmp);
		add(m);

		m = new JMenu("Accentuation");
		
		tmp = new Sharpen();
		mi = new JMenuItem(tmp.toString());
		mi.addActionListener(this);
		m.add(mi);
		ash.put(mi, tmp);
		add(m);

		m = new JMenu("Gradient");

		tmp = new Sobelien();
		mi = new JMenuItem(tmp.toString());
		mi.addActionListener(this);
		m.add(mi);
		ash.put(mi, tmp);

		tmp = new Roberts();
		mi = new JMenuItem(tmp.toString());
		mi.addActionListener(this);
		m.add(mi);
		ash.put(mi, tmp);

		tmp = new Prewitt();
		mi = new JMenuItem(tmp.toString());
		mi.addActionListener(this);
		m.add(mi);
		ash.put(mi, tmp);

		tmp = new Freichen();
		mi = new JMenuItem(tmp.toString());
		mi.addActionListener(this);
		m.add(mi);
		ash.put(mi, tmp);

		add(m);

		m = new JMenu("Niveaux");

		tmp = new SeuillageIn();
		mi = new JMenuItem(tmp.toString());
		mi.addActionListener(this);
		m.add(mi);
		ash.put(mi, tmp);

		tmp = new SeuillageOut();
		mi = new JMenuItem(tmp.toString());
		mi.addActionListener(this);
		m.add(mi);
		ash.put(mi, tmp);

		tmp = new Rescale();
		mi = new JMenuItem(tmp.toString());
		mi.addActionListener(this);
		m.add(mi);
		ash.put(mi, tmp);

		add(m);

		m = new JMenu("Watershed");

		tmp = new WatershedAutoHmin();
		mi = new JMenuItem(tmp.toString());
		mi.addActionListener(this);
		m.add(mi);
		ash.put(mi, tmp);

		tmp = new WatershedManuel();
		mi = new JMenuItem(tmp.toString());
		mi.addActionListener(this);
		m.add(mi);
		ash.put(mi, tmp);

		tmp = new WatershedStat();
		mi = new JMenuItem(tmp.toString());
		mi.addActionListener(this);
		m.add(mi);
		ash.put(mi, tmp);

		add(m);

		m = new JMenu("Fusion");

		tmp = new Fusion();
		mi = new JMenuItem(tmp.toString());
		mi.addActionListener(this);
		m.add(mi);
		ash.put(mi, tmp);

		tmp = new FPrecedent();
		mi = new JMenuItem(tmp.toString());
		mi.addActionListener(this);
		m.add(mi);
		ash.put(mi, tmp);

		add(m);

		addMenuListener(this);
	}

	public void menuCanceled(MenuEvent me)
	{
		
	}

	public void menuDeselected(MenuEvent me)
	{
		
	}

	public void menuSelected(MenuEvent me) {
		setPopupMenuVisible(true);
		refresh();
	}

	public void refresh() {
		Enumeration enum = ash.keys();
		Picture p = waz.getPicture();

		if (p != null) {
			JMenuItem mi = null;
			boolean actif = false;

			while(enum.hasMoreElements()) {
				mi = (JMenuItem)enum.nextElement();
				actif = ((Filter)ash.get(mi)).isApplicable(p);
				mi.setEnabled(actif);
			}
		}
		else
			while(enum.hasMoreElements())
				((JMenuItem)(enum.nextElement())).setEnabled(false);
	}

	public void actionPerformed(ActionEvent e) {
		JMenuItem mi = (JMenuItem)(e.getSource());

		Filter f = (Filter)(ash.get(mi));
		if (f != null) {
			try {
				Picture ret = f.execute(waz.getPicture());
				waz.createPicture(ret);
			}
			catch (OutOfMemoryError mem) {
				waz.erreur.show(mi, mem, waz.lng.E_NOMEM, mem.toString(), waz.lng.DB_OK);
			}
		}
	}
	
	public void reload() {
		setText(waz.lng.M_FILTERS);
	}
}