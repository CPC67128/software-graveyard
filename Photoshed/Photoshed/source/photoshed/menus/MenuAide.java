package photoshed.menus;

import javax.swing.JMenu;
import javax.swing.JMenuItem;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

import photoshed.Photoshed;
import photoshed.menus.Reloadable;

public final class MenuAide extends JMenu implements ActionListener, Reloadable {
	private Photoshed waz;

	protected MenuAide(Photoshed w) {
		super(w.lng.M_HELP);
		waz  = w;
		JMenuItem mi;

		mi = new JMenuItem(w.lng.M_HELP_CONTENTS);
		mi.addActionListener(this);
		add(mi);
		
		addSeparator();
		
		mi = new JMenuItem(w.lng.M_HELP_ABOUT);
		mi.addActionListener(this);
		add(mi);

	}

	public void actionPerformed(ActionEvent ae) {
		JMenuItem mi = (JMenuItem)(ae.getSource());

		if (waz.lng.M_HELP_CONTENTS.equals(mi.getText()))
			waz.showContents();
		else if (waz.lng.M_HELP_ABOUT.equals(mi.getText()))
			waz.showAbout();
		else {
		
		}
	}

	public void reload() {
		setText(waz.lng.M_HELP);
		removeAll();
		JMenuItem mi;

		mi = new JMenuItem(waz.lng.M_HELP_CONTENTS);
		mi.addActionListener(this);
		add(mi);
		
		addSeparator();
		
		mi = new JMenuItem(waz.lng.M_HELP_ABOUT);
		mi.addActionListener(this);
		add(mi);
				
	}
}
