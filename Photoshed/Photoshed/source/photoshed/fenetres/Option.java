package photoshed.fenetres;

import java.awt.GridLayout;
import java.awt.FlowLayout;
import java.awt.event.WindowListener;
import java.awt.event.WindowEvent;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.event.ItemListener;
import java.awt.event.ItemEvent;

import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.JButton;
import javax.swing.JPanel;

import java.util.Properties;
import java.util.Vector;

import java.io.File;

import photoshed.utils.Langage;

public final class Option extends JComponent implements WindowListener, ActionListener, ItemListener {

	private JLabel lblLanguage;

	private JComboBox lstLanguage;

	private JLabel lblNbLast;

	private JTextField txtNbLast;

	private JButton clearLast;

	private JCheckBox saveWindowLocation;

	private JCheckBox openColor;

	private JCheckBox FilterNewWindow;

	private JCheckBox showSplash;

	private JButton ok;

	private JButton cancel;

	private JButton apply;

	private Langage lng;

	private Properties ppt;

	private JDialog dialog;

	private boolean construction;

	private int nbLast;

	private boolean color;

	private boolean saveWin;

	private boolean newWin;

	private boolean splash;

	private String lngF;

	private boolean change;

	public Option() {
		setLayout(new GridLayout(0, 1));
		JPanel tmp = new JPanel(new FlowLayout());
		lblLanguage = new JLabel();
		lstLanguage = new JComboBox();
		lstLanguage.setEditable(false);
		lstLanguage.addActionListener(this);
		tmp.add(lblLanguage);
		tmp.add(lstLanguage);
		add(tmp);

		tmp = new JPanel(new FlowLayout());
		lblNbLast = new JLabel();
		txtNbLast = new JTextField(2);
		txtNbLast.addActionListener(this);
		tmp.add(lblNbLast);
		tmp.add(txtNbLast);
		add(tmp);

		tmp = new JPanel(new FlowLayout());
		clearLast = new JButton();
		clearLast.addActionListener(this);
		tmp.add(clearLast);
		add(tmp);

		tmp = new JPanel(new FlowLayout());
		saveWindowLocation = new JCheckBox();
		saveWindowLocation.addItemListener(this);
		tmp.add(saveWindowLocation);
		add(tmp);

		tmp = new JPanel(new FlowLayout());
		openColor = new JCheckBox();
		openColor.addItemListener(this);
		tmp.add(openColor);
		add(tmp);

		tmp = new JPanel(new FlowLayout());
		FilterNewWindow = new JCheckBox();
		FilterNewWindow.addItemListener(this);
		tmp.add(FilterNewWindow);
		add(tmp);

		tmp = new JPanel(new FlowLayout());
		showSplash = new JCheckBox();
		showSplash.addItemListener(this);
		tmp.add(showSplash);
		add(tmp);

		tmp = new JPanel(new FlowLayout());
		ok = new JButton();
		ok.addActionListener(this);
		cancel = new JButton();
		cancel.addActionListener(this);
		apply = new JButton();
		apply.addActionListener(this);
		tmp.add(ok);
		tmp.add(cancel);
		tmp.add(apply);
		add(tmp);

		construction = false;
	}

	public Properties showOption(JFrame parent, Properties p, Langage l) {
		lng = l;
		ppt = p;
		Properties pp = null;

		change = false;
		nbLast = Integer.parseInt(p.getProperty("file.last.nb", "4"));
		color = p.getProperty("file.open.color", "false").equals("true");
		saveWin = p.getProperty("window.save", "true").equals("true");
		newWin = p.getProperty("appli.newpic", "true").equals("true");
		splash = p.getProperty("appli.showsplash", "true").equals("true");
		lngF = l.G_LANGAGE_FILENAME;

		dialog = new JDialog(parent, lng.DB_OPTION, true);
		apply.setEnabled(false);

		initComponent();

		dialog.setDefaultCloseOperation(javax.swing.WindowConstants.DO_NOTHING_ON_CLOSE);
		dialog.addWindowListener(this);
		dialog.setContentPane(this);
		dialog.pack();
		dialog.setLocationRelativeTo(parent);
		dialog.show();

		if (change)
			pp = ppt;
		lng = null;
		ppt = null;
		dialog = null;
		return pp;
	}

	private void initComponent() {
		construction = true;
		//txtNbLast.setText(ppt.getProperty("file.last.nb", "4"));
		txtNbLast.setText("" + nbLast);

		String appPath = System.getProperty("user.dir");
		String s = null;
		int index = 0;
		int j = 0;
		String [] tmp = (new File(appPath)).list();
		lstLanguage.removeAllItems();
		for (int i = 0, k = 0; i < tmp.length; i++) {
			index = tmp[i].lastIndexOf(".wlg");
			if (index > 0) {
				s = tmp[i].substring(0, index);
				/*if (s.equals(lng.G_LANGAGE_FILENAME))
					j = k;*/
				lstLanguage.addItem(s);
				k++;
			}
		}
		/*lstLanguage.setSelectedIndex(j);*/
		lstLanguage.setSelectedItem(lng.G_LANGAGE_FILENAME);
		lblLanguage.setText(lng.DB_LNGSELECT);

		lblNbLast.setText(lng.DB_RECENT);

		clearLast.setText(lng.DB_CLEARLAST);

		saveWindowLocation.setText(lng.DB_SAVEWIN);
		//saveWindowLocation.setSelected("true".equals(ppt.getProperty("window.save", "true")));
		saveWindowLocation.setSelected(saveWin);

		openColor.setText(lng.DB_COLOR);
		//openColor.setEnabled("true".equals(ppt.getProperty("file.open.color", "false")));
		openColor.setSelected(color);

		FilterNewWindow.setText(lng.DB_PIC);
		//FilterNewWindow.setSelected("true".equals(ppt.getProperty("appli.newpic", "true")));
		FilterNewWindow.setSelected(newWin);

		showSplash.setText(lng.DB_SHOWSPLASH);
		//showSplash.setSelected("true".equals(ppt.getProperty("appli.showsplash", "true")));
		showSplash.setSelected(splash);

		ok.setText(lng.DB_OK);
		cancel.setText(lng.DB_CANCEL);
		apply.setText(lng.DB_APPLY);
		construction = false;
	}

	public void windowActivated(WindowEvent e) {}
	public void windowClosed(WindowEvent e) {}
	public void windowDeactivated(WindowEvent e) {}
	public void windowDeiconified(WindowEvent e) {}
	public void windowIconified(WindowEvent e) {}
	public void windowOpened(WindowEvent e) {}
	public void windowClosing(WindowEvent e) {
		dialog.remove(this);
		dialog.dispose();
	}
	public void actionPerformed(ActionEvent e) {
		if (!construction) {
			JComponent jc = (JComponent)e.getSource();
			if (jc instanceof JComboBox) {
				lngF = (String)lstLanguage.getSelectedItem();
				lng.setLangage(lngF);
				initComponent();
				change = true;
				apply.setEnabled(true);
			}
			else if (jc instanceof JButton) {
				JButton jb = (JButton)jc;
				if (jb == clearLast) {
					for (int i = 9; i >= 0; i--)
						ppt.remove("file.last." + i);
				}
				else if (jb == apply) {
					construction = true;
					ppt.setProperty("file.last.nb", "" + nbLast);
					ppt.setProperty("window.save", "" + saveWin);
					ppt.setProperty("file.open.color", "" + color);
					ppt.setProperty("appli.newpic", "" + newWin);
					ppt.setProperty("appli.showsplash", "" + splash);
					ppt.setProperty("appli.language", lngF);
					change = true;
					jb.setEnabled(false);
					construction = false;
				}
				else {
					if (jb == cancel) {
						String l = ppt.getProperty("appli.language");
						if (!lngF.equals(l))
							lng.setLangage(l);
						ppt = null;
					}
					else {
						ppt.setProperty("file.last.nb", "" + nbLast);
						ppt.setProperty("window.save", "" + saveWin);
						ppt.setProperty("file.open.color", "" + color);
						ppt.setProperty("appli.newpic", "" + newWin);
						ppt.setProperty("appli.showsplash", "" + splash);
						ppt.setProperty("appli.language", lngF);
					}
					dialog.remove(this);
					dialog.dispose();
				}
			}
			else if (jc instanceof JTextField) {
				int nb = 0;
				try {
					nb = Integer.parseInt(txtNbLast.getText());
				} catch (Exception ex) {}
				if (nb > 0 && nb < 10) {
					nbLast = nb;
					change = true;
					apply.setEnabled(true);
				} else {
					construction = true;
					txtNbLast.setText("" + nbLast);
					construction = false;
				}
			}
		}
	}

	public void itemStateChanged(ItemEvent e) {
		if (!construction) {
			JCheckBox jcb = (JCheckBox)e.getSource();
			boolean state = jcb.isSelected();
			if (jcb == saveWindowLocation)
				saveWin = state;
			else if (jcb == openColor)
				color = state;
			else if (jcb == FilterNewWindow)
				newWin = state;
			else if (jcb == showSplash)
				splash = state;
			change = true;
			apply.setEnabled(true);
		}
	}
}