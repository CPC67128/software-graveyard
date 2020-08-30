package photoshed.filtres;

import java.awt.image.BufferedImage;
import java.awt.event.ActionListener;
import java.awt.event.ItemListener;
import java.awt.event.WindowListener;

import javax.swing.JCheckBox;
import javax.swing.JDialog;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.ImageIcon;

import photoshed.fenetres.Picture;

public abstract class StdFilter {

	protected JButton jbOK = null;

	protected JButton jbCancel = null;

	protected JButton jbReset = null;

	protected JDialog jdFiltre = null;

	protected JLabel lbl = null;

	protected ImageIcon iicon = null;

	protected JCheckBox jcbAperçu = null;

	protected BufferedImage src = null;

	protected BufferedImage dst = null;

	protected Picture retPic = null;

	protected String name = null;

	protected boolean firstTime = true;

	public StdFilter(boolean aUnAperçu) {

		iicon = new ImageIcon();
		lbl = new JLabel(iicon);

		jbOK = new JButton("Appliquer");
		
		jbCancel = new JButton("Annuler");

		jbReset = new JButton("Réinitialiser");

		if (aUnAperçu) {
			jcbAperçu = new JCheckBox("Aperçu", false);
		}
	}

	protected void addListener(ActionListener button, ItemListener checkbox) {
		if (checkbox != null && jcbAperçu != null) {
			jcbAperçu.addItemListener(checkbox);
		}
		if (button != null) {
			if (jbOK != null) {
				jbOK.addActionListener(button);
			}
			if (jbCancel != null) {
				jbCancel.addActionListener(button);
			}
			if (jbReset != null) {
				jbReset.addActionListener(button);
			}
		}
	}

	protected void addWindowListener(WindowListener jdialog) {
		if (jdialog != null && jdFiltre != null) {
			jdFiltre.setDefaultCloseOperation(javax.swing.WindowConstants.DO_NOTHING_ON_CLOSE);
			jdFiltre.addWindowListener(jdialog);
		}
	}

}