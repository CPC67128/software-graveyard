package photoshed.filtres;

import java.awt.image.BufferedImage;
import java.awt.image.DataBuffer;
import java.awt.image.SampleModel;
import java.awt.image.WritableRaster;
import java.awt.Container;
import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemListener;
import java.awt.event.ItemEvent;
import java.awt.event.WindowListener;
import java.awt.event.WindowEvent;

import javax.swing.JDialog;
import javax.swing.JButton;
import javax.swing.JSlider;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JLabel;
import javax.swing.ImageIcon;
import javax.swing.event.ChangeListener;
import javax.swing.event.ChangeEvent;

import photoshed.fenetres.Picture;
import photoshed.filtres.Filter;
import photoshed.filtres.StdFilter;

/**
 *
 *
 *
 */

public final class Sharpen extends StdFilter implements Filter, ActionListener, ChangeListener, ItemListener, WindowListener {

	private JSlider jsLvl;

	private int level = 50;

	public Sharpen() {

		super(true);
		addListener(this, this);

		jsLvl = new JSlider(JSlider.HORIZONTAL, 50, 200, level);
		jsLvl.addChangeListener(this);
	}

	/**
	 *
	 *
	 *
	 */

	public boolean isApplicable(Picture pic){
		return true;
	}

	/**
	 *
	 *
	 *
	 */

	public Picture execute(Picture pic){
		if (pic == null)
			return null;

		src = pic.getSource();
		dst = new BufferedImage(src.getWidth(), src.getHeight(), src.getType());
		if (jcbAperçu.isSelected())
			iicon.setImage(traitement(src, dst));
		else
			iicon.setImage(dst);
		lbl.revalidate();

		name = pic.getName();

		if (firstTime) {

			java.awt.Component ctmp = pic.getDesktopPane();
			while(!(ctmp instanceof JFrame)) {
				ctmp = ctmp.getParent();
			}

			JFrame tmp = (JFrame)(ctmp);
			jdFiltre = new JDialog(tmp, toString(), true);
			Container c = jdFiltre.getContentPane();

			JScrollPane scroll = new JScrollPane();
			scroll.getViewport().add(lbl);
			scroll.setVisible(true);

			JPanel top = new JPanel(new GridLayout(2, 2));
			top.add(jbOK);
			top.add(jbCancel);
			top.add(jbReset);
			top.add(jcbAperçu);

			JPanel bottom = new JPanel(new GridLayout(1, 2));
			bottom.add(new JLabel("Niveau : "));
			bottom.add(jsLvl);

			c.setLayout(new BorderLayout());
			c.add(top, BorderLayout.NORTH);
			c.add(bottom, BorderLayout.SOUTH);
			c.add(scroll, BorderLayout.CENTER);
			addWindowListener(this);
			firstTime = false;
		}
		jdFiltre.pack();
		jdFiltre.setLocationRelativeTo(pic);
		jdFiltre.setVisible(true);

		if (retPic != null)
			retPic.property = pic.property;

		return retPic;
	}

	/**
	 *
	 *
	 *
	 */

	public BufferedImage traitement(BufferedImage src, BufferedImage dst) {

		int w = src.getWidth();
		int h = src.getHeight();
		int i, j, k;
		SampleModel smSRC, smDST;
		WritableRaster wrSRC, wrDST;
		DataBuffer dbSRC, dbDST;
		wrSRC = src.getRaster();
		wrDST = dst.getRaster();
		smSRC = wrSRC.getSampleModel();
		smDST = wrDST.getSampleModel();
		dbSRC = wrSRC.getDataBuffer();
		dbDST = wrDST.getDataBuffer();
		
		int [] source = new int[w * h];
		int [][] destin = new int[h][w];
		float coef1 = (level - 25)/ 5.0F;
		float coef2 = coef1 / -5.0F;
		float add;
for (int b = smSRC.getNumBands() - 1; b >= 0; b--) {
		smSRC.getSamples(0, 0, w, h, b, source, dbSRC);

		for (i = 0, k = 0; i < h; i++) {
			for (j = 0; j < w; j++, k++) {
				destin[i][j] = source[k];
			}
		}

		for (i = 0, k = 0; i < h; i++) {
			for (j = 0; j < w; j++, k++) {
				if (i > 0 && i < h - 1 && j > 0 && j < w - 1) {
					add = destin[i-1][j] * coef2 + 
							+ destin[i][j-1] * coef2 + destin[i][j] * coef1 + destin[i][j+1] * coef2
							+ destin[i+1][j] * coef2;
					add = add / (coef1 + 4 * coef2);
					add = (float)Math.sqrt(add * add);
					if (add > 255)
						source[k] = 255;
					else
						source[k] = (int)add;
				}
				else {
					source[k] = destin[i][j];
				}
			}
		}

		smDST.setSamples(0, 0, w, h, b, source, dbDST);
}
		return dst;
	}

	/**
	 *
	 *
	 *
	 */

	public String toString() {
		return "Sharpen ...";
	}

	public void stateChanged(ChangeEvent e) {
		JSlider source = (JSlider)e.getSource();
		int value = source.getValue();

		if (source == jsLvl)
			level = value;

		if (jcbAperçu.isSelected()) {
			traitement(src, dst);
			lbl.repaint();
		}	
	}

	public void actionPerformed(ActionEvent e) {
		JButton jb = (JButton)(e.getSource());
		if (jb == jbReset) {
			boolean tmp = jcbAperçu.isSelected();
			jcbAperçu.setSelected(false);
			jsLvl.setValue(50);
			jcbAperçu.setSelected(tmp);
		}
		else {
			if (jb == jbOK) {
				retPic = new Picture(name, traitement(src, dst));
			}
			else {
				retPic = null;
			}
			jdFiltre.setVisible(false);
		}
	}

	public void itemStateChanged(ItemEvent e) {
		if (jcbAperçu.isSelected()) {
			traitement(src, dst);
			lbl.repaint();
		}
	}

	public void windowActivated(WindowEvent e) {}
	public void windowClosed(WindowEvent e) {}
	public void windowClosing(WindowEvent e) {
		retPic = null;
		jdFiltre.setVisible(false);
	}
	public void windowDeactivated(WindowEvent e) {}
	public void windowDeiconified(WindowEvent e) {}
	public void windowIconified(WindowEvent e) {}
	public void windowOpened(WindowEvent e) {}
}