package photoshed.filtres;

import java.awt.image.BufferedImage;
import java.awt.image.DataBuffer;
import java.awt.image.SampleModel;
import java.awt.image.WritableRaster;
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

public final class Rescale extends StdFilter implements Filter, ActionListener, ChangeListener, ItemListener, WindowListener {

	private static final String M = "Multiplication : ";

	private static final String A = "Addition : ";

	private JSlider jsMult;

	private JSlider jsAdd;

	private JLabel lblMult;

	private JLabel lblAdd;

	private int multi = 0;

	private int addit = 0;

	public Rescale() {

		super(true);
		addListener(this, this);

		jsMult = new JSlider(JSlider.HORIZONTAL, -400, 700, multi);
		jsMult.addChangeListener(this);

		jsAdd = new JSlider(JSlider.HORIZONTAL, -255, 255, addit);
		jsAdd.addChangeListener(this);

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
			java.awt.Container c = jdFiltre.getContentPane();

			JScrollPane scroll = new JScrollPane();
			scroll.getViewport().add(lbl);
			scroll.setVisible(true);

			JPanel top = new JPanel(new GridLayout(2, 2));
			top.add(jbOK);
			top.add(jbCancel);
			top.add(jbReset);
			top.add(jcbAperçu);

			JPanel bottom = new JPanel(new GridLayout(4, 1));
			bottom.add(lblMult = new JLabel(M + " x " + jsMult.getValue()));
			bottom.add(jsMult);
			bottom.add(lblAdd = new JLabel(A + " + " + jsAdd.getValue()));
			bottom.add(jsAdd);

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
		int k, scale;
		SampleModel smSRC, smDST;
		WritableRaster wrSRC, wrDST;
		DataBuffer dbSRC, dbDST;
		wrSRC = src.getRaster();
		wrDST = dst.getRaster();
		smSRC = wrSRC.getSampleModel();
		smDST = wrDST.getSampleModel();
		dbSRC = wrSRC.getDataBuffer();
		dbDST = wrDST.getDataBuffer();
		float m;
		
		int [] source = new int[w * h];
for (int i = smSRC.getNumBands() - 1; i >= 0; i--) {
		smSRC.getSamples(0, 0, w, h, i, source, dbSRC);

		if (multi >= 0)
			m = (multi/10.0F) + 1;
		else
			m = 1 / ((Math.abs(multi) / 10.0F) + 1);

		for (k = 0; k < source.length; k++) {
			scale = (int)(source[k] * m + addit);
			if (scale > 255)
				scale = 255;
			else if (scale < 0)
				scale = 0;

			source[k] = scale;
		}

		smDST.setSamples(0, 0, w, h, i, source, dbDST);
}
		return dst;
	}

	/**
	 *
	 *
	 *
	 */

	public String toString() {
		return "Mise à niveau ...";
	}

	public void stateChanged(ChangeEvent e) {
		JSlider source = (JSlider)e.getSource();
		int value = source.getValue();

		if (source == jsMult) {
			multi = value;
			lblMult.setText(M + " + " + jsMult.getValue());
		}
		else {
			addit = value;
			lblAdd.setText(A + " + " + jsAdd.getValue());
		}
		
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
			jsMult.setValue(0);
			jsAdd.setValue(0);
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