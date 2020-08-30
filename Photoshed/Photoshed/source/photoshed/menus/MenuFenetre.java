package photoshed.menus;

import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.event.MenuListener;
import javax.swing.event.MenuEvent;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

import photoshed.Photoshed;
import photoshed.fenetres.Picture;
import photoshed.menus.Reloadable;

public final class MenuFenetre extends JMenu implements ActionListener, MenuListener, Reloadable {
	private Photoshed waz = null;

	private JMenuItem tile;

	private JMenuItem cascade;

	private JMenuItem closeall;

	protected MenuFenetre(Photoshed w) {
		super(w.lng.M_WINDOW);
		waz = w;

		tile = new JMenuItem(w.lng.M_WINDOW_TILE);
		tile.addActionListener(this);
		add(tile);

		cascade = new JMenuItem(w.lng.M_WINDOW_CASCADE);
		cascade.addActionListener(this);
		add(cascade);
		
		closeall = new JMenuItem(w.lng.M_WINDOW_CLOSEALL);
		closeall.addActionListener(this);
		add(closeall);

		addMenuListener(this);
	}

	public void actionPerformed(ActionEvent ae) {

		JMenuItem mi = (JMenuItem)(ae.getSource());

		if (tile == mi)
			waz.tile();
		else if (cascade == mi)
			waz.cascade();
		else if (closeall  == mi)
			waz.closeAll();
		else {
			waz.setPicture(Integer.parseInt(mi.getText().substring(0,1)) - 1);
		}
	}

	public void menuCanceled(MenuEvent me)
	{
		
	}

	public void menuDeselected(MenuEvent me)
	{
		
	}

	public void menuSelected(MenuEvent me)
	{
		removeAll();
		add(tile);
		add(cascade);
		add(closeall);

		if (waz.isPicture()) {
			tile.setEnabled(true);
			cascade.setEnabled(true);
			closeall.setEnabled(true);
			addSeparator();
			JMenuItem mi;
			Picture [] imgs = waz.getPictures();
			for ( int i = 0; i < imgs.length; i++) {
				mi = new JMenuItem((i + 1) + " - " + imgs[i].getName());
				mi.addActionListener(this);
				add(mi);
			}
		}
		else {
			tile.setEnabled(false);
			cascade.setEnabled(false);
			closeall.setEnabled(false);
		}
	}

	public void reload() {
		setText(waz.lng.M_WINDOW);
		tile.setText(waz.lng.M_WINDOW_TILE);
		cascade.setText(waz.lng.M_WINDOW_CASCADE);
		closeall.setText(waz.lng.M_WINDOW_CLOSEALL);
	}
}