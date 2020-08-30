package photoshed.menus;

import java.awt.Component;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.InputEvent;

import javax.swing.JMenuBar;
import javax.swing.MenuElement;

import java.io.File;

import photoshed.menus.MenuFiltre;
import photoshed.menus.MenuAide;
import photoshed.menus.MenuFenetre;
import photoshed.menus.MenuFichier;
import photoshed.menus.Reloadable;
import photoshed.Photoshed;
import photoshed.utils.Langage;

public final class PhotoshedMenuBar extends JMenuBar implements MouseListener {

	MenuFiltre filtres;

	public PhotoshedMenuBar(Photoshed w) {
		try {
			add(new MenuFichier(w));
			filtres = new MenuFiltre(w);
			add(filtres);
			add(new MenuFenetre(w));
			add(new MenuAide(w));
		}
		catch(Exception ex) {
			w.erreur.show(this, ex, w.lng.E_STOP, w.lng.E_LOAD_MENU + "\n" + ex.toString(), w.lng.DB_OK);
			w.exit();
		}
	}

	public void reload() {
		MenuElement [] me = getSubElements();
		for (int i = me.length - 1; i >= 0; i--)
			((Reloadable)me[i]).reload();
	}

	public void mouseClicked(MouseEvent e) {}
	public void mouseEntered(MouseEvent e) {}
	public void mouseExited(MouseEvent e) {}
	public void mousePressed(MouseEvent e) {
		if (e.getModifiers() != InputEvent.BUTTON1_MASK) {
			filtres.refresh();
			filtres.getPopupMenu().show((Component)e.getSource(), e.getX(), e.getY());
		}
	}
	public void mouseReleased(MouseEvent e) {}

}