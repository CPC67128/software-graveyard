package photoshed.menus;

import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.KeyStroke;
import javax.swing.event.MenuListener;
import javax.swing.event.MenuEvent;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.Event;

import java.io.File;

import photoshed.Photoshed;
import photoshed.menus.Reloadable;

public final class MenuFichier extends JMenu implements ActionListener, MenuListener, Reloadable {
	private Photoshed waz;

	private JMenuItem open;

	private JMenuItem save;

	private JMenuItem saveas;

	private JMenuItem close;

	private JMenuItem option;

	private JMenu recent;

	private JMenuItem quit;

	protected MenuFichier(Photoshed w) {
		super(w.lng.M_FILE);
		waz = w;

		open = new JMenuItem(w.lng.M_FILE_OPEN);
		open.addActionListener(this);
		open.setAccelerator(KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_O, Event.CTRL_MASK));
		add(open);
		
		save = new JMenuItem(w.lng.M_FILE_SAVE);
		save.addActionListener(this);
		save.setAccelerator(KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_S, Event.CTRL_MASK));
		add(save);
		
		saveas = new JMenuItem(w.lng.M_FILE_SAVEAS);
		saveas.addActionListener(this);
		add(saveas);

		close = new JMenuItem(w.lng.M_FILE_CLOSE);
		close.addActionListener(this);
		add(close);

		addSeparator();

		option = new JMenuItem(w.lng.M_FILE_OPTION);
		option.addActionListener(this);
		add(option);

		addSeparator();

		recent = new JMenu(w.lng.M_FILE_RECENT);
		recent.addMenuListener(this);
		add(recent);

		addSeparator();

		quit = new JMenuItem(w.lng.M_FILE_QUIT);
		quit.addActionListener(this);
		quit.setAccelerator(KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_Q, Event.CTRL_MASK));
		add(quit);

		addMenuListener(this);
	}

	public void actionPerformed(ActionEvent ae) {

		JMenuItem mi = (JMenuItem)(ae.getSource());

		if (open == mi)
			waz.showOpenDialog();
		else if (save == mi)
			waz.saveCurrent();
		else if (saveas == mi)
			waz.showSaveAsDialog();
		else if (close == mi)
			waz.closeCurrent();
		else if (option == mi)
			waz.showOption();
		else if (quit == mi)
			waz.exit();
		else
			waz.open(new File(mi.getText()));
	}

	public void menuCanceled(MenuEvent me) {}

	public void menuDeselected(MenuEvent me) {}

	public void menuSelected(MenuEvent me)
	{
		JMenu m = (JMenu)me.getSource();

		if (m == recent) {
			int max = Integer.parseInt(waz.proprietes.getProperty("file.last.nb", "4"));
			String name = null;
			JMenuItem mi = null;
			recent.removeAll();
			for (int i = 0; i < max; i++) {
				name = waz.proprietes.getProperty("file.last." + i);
				if (name != null) {
					mi = new JMenuItem(name);
					mi.addActionListener(this);
					recent.add(mi);
				}
			}
		}
		else {
			boolean actif = false;
			if (waz.isPictureSelected())
				actif = true;

			save.setEnabled(actif);
			saveas.setEnabled(actif);
			close.setEnabled(actif);
		}
	}

	public void reload() {
		setText(waz.lng.M_FILE);
		open.setText(waz.lng.M_FILE_OPEN);
		save.setText(waz.lng.M_FILE_SAVE);
		saveas.setText(waz.lng.M_FILE_SAVEAS);
		close.setText(waz.lng.M_FILE_CLOSE);
		option.setText(waz.lng.M_FILE_OPTION);
		recent.setText(waz.lng.M_FILE_RECENT);
		quit.setText(waz.lng.M_FILE_QUIT);
	}
}