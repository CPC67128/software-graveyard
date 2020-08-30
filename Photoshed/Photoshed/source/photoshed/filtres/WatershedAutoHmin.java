package photoshed.filtres;

import java.awt.image.BufferedImage;
import java.awt.image.DataBuffer;
import java.awt.image.SampleModel;
import java.awt.image.WritableRaster;
import java.awt.image.ColorConvertOp;
import java.awt.Container;
import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemListener;
import java.awt.event.ItemEvent;
import java.awt.event.WindowListener;
import java.awt.event.WindowEvent;

import java.util.LinkedList;

import javax.swing.JCheckBox;
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
import photoshed.filtres.watershed.WS;
import photoshed.utils.Pile;

/**
 *
 *
 *
 */

public final class WatershedAutoHmin extends StdFilter implements Filter, ActionListener, ChangeListener, ItemListener, WindowListener {

	private JSlider jsHmin;

	private int hmin = 20;

	private int waterLabel = 0;

	private boolean toutFaire;

	private final int MASK = 0x2f2f2f;

	public WatershedAutoHmin() {

		super(true);
		addListener(this, this);

		jsHmin = new JSlider(JSlider.HORIZONTAL, 0, 255, hmin);
		jsHmin.addChangeListener(this);

		toutFaire = false;
	}

	/**
	 *
	 *
	 *
	 */

	public boolean isApplicable(Picture pic){
		return pic.property instanceof BufferedImage;
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

		int w = src.getWidth(),
			h = src.getHeight();

		if (src.getType() != BufferedImage.TYPE_BYTE_GRAY) 
			src = new ColorConvertOp(null).filter(src, new BufferedImage(w, h, BufferedImage.TYPE_BYTE_GRAY));
		dst = new BufferedImage(w, h, BufferedImage.TYPE_3BYTE_BGR);
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

			JPanel bottom = new JPanel(new GridLayout(2, 1));
			bottom.add(new JLabel("H minima : "));
			bottom.add(jsHmin);

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
			retPic.property = new WS((BufferedImage)pic.property, pic.getSource(), waterLabel);

		return retPic;
	}

	/**
	 *
	 *
	 *
	 */

	public BufferedImage traitement(BufferedImage src, BufferedImage dst) {

		SampleModel smSRC, smDST;
		WritableRaster wrSRC, wrDST;
		DataBuffer dbSRC, dbDST;
		
		wrSRC = src.getRaster();
		wrDST = dst.getRaster();
		smSRC = wrSRC.getSampleModel();
		smDST = wrDST.getSampleModel();
		dbSRC = wrSRC.getDataBuffer();
		dbDST = wrDST.getDataBuffer();
		
		int w = src.getWidth();
		int h = src.getHeight();
		int g, i, j, k, l;
		int x, y, x1, x2, y1, y2;
		int min, max;
		int label, DM;

		boolean onLeft, onTop, onRight, onBottom, changements;

		int [] source = new int[w * h];
		int [][] tab = new int[h][w];
		int [][] labels = new int[h][w];

		smSRC.getSamples(0, 0, w, h, 0, source, dbSRC);
		
		max = tab[0][0];
		min = tab[0][0];

		for (j = h - 1, l = w * h - 1; j >= 0; j--)
			for (i = w - 1; i >= 0; i--, l--) {
				k = tab[j][i] = source[l];
				max = (max<k)?(k):(max);
				min = (min>k)?(k):(min);
			}

		DM = hmin * (max-min) / 255;
		
		label = 1;
		
		for (g = min; g <= min + DM; g++)
		{
			for (y = 0; y < h; y++)
				for (x = 0; x < w; x++)
					if (labels[y][x] == 0 && tab[y][x]<=g)
					{
						k = label;
						labels[y][x] = k;

						x1 = (x>0)?(x-1):(x);
						y1 = (y>0)?(y-1):(y);
						x2 = (x<w)?(x+1):(x);
						y2 = (y<h)?(y+1):(y);

						changements = true;

						while (changements)
						{
							onLeft      = false;
							onRight     = false;
							onTop       = false;
							onBottom    = false;
							changements = false;

							for (j = y1; j < y2; j++)
								for (i = x1; i < x2; i++)
									if (labels[j][i]==k)
									{
										if (i>0 && labels[j][i-1]==0 && tab[j][i-1]<=g+DM)
										{
											labels[j][i-1] = k;
											changements = true;
											onLeft = true;
										}

										if (i<w-1 && labels[j][i+1]==0 && tab[j][i+1]<=g+DM)
										{
											labels[j][i+1] = k;
											changements = true;
											onRight = true;
										}

										if (j>0 && labels[j-1][i]==0 && tab[j-1][i]<=g+DM)
										{
											labels[j-1][i] = k;
											changements = true;
											onTop = true;
										}
										
										if (j<h-1 && labels[j+1][i]==0 && tab[j+1][i]<=g+DM)
										{
											labels[j+1][i] = k;
											changements = true;
											onBottom = true;
										}
									}

							if (onLeft)   x1 = (x1>0)?(x1-1):(x1);
							if (onRight)  x2 = (x2<w)?(x2+1):(x2);
							if (onTop)    y1 = (y1>0)?(y1-1):(y1);
							if (onBottom) y2 = (y2<h)?(y2+1):(y2);

						}
						label++;
					}
		}
		waterLabel = label;

		if (toutFaire) {

			Point p = null;
			
			Pile [] niveau = new Pile[256];

			for (i = 255; i >= 0; i--)
				niveau[i] = new Pile();

			for (j = h - 1 ; j >= 0; j--)
				for (i = w - 1 ; i >= 0 ; i--)
					if (labels[j][i] != 0)
						niveau[tab[j][i]].push((Object)new Point(i, j));

			for (g = 0; g < 256; g++) {
				while (!niveau[g].isEmpty()) {
					p = (Point)niveau[g].pop();
					x = p.x;
					y = p.y;
					label = labels[y][x];
					if (x>0 && labels[y][x-1]==0) {
						labels[y][x-1] = label;
						l = (tab[y][x-1] > g)?(tab[y][x-1]):(g);
						niveau[l].push((Object)new Point(x-1, y));
					}

					if (x<w-1 && labels[y][x+1]==0) {
						labels[y][x+1] = label;
						l = (tab[y][x+1] > g)?(tab[y][x+1]):(g);
						niveau[l].push((Object)new Point(x+1, y));
					}

					if (y>0 && labels[y-1][x]==0) {
						labels[y-1][x] = label;
						l = (tab[y-1][x] > g)?(tab[y-1][x]):(g);
						niveau[l].push((Object)new Point(x, y-1));
					}

					if (y<h-1 && labels[y+1][x]==0) {
						labels[y+1][x] = label;
						l = (tab[y+1][x] > g)?(tab[y+1][x]):(g);
						niveau[l].push((Object)new Point(x, y+1));
					}
				}
				niveau[g] = null;
			}
			for (j = h - 1, l = w * h - 1; j >= 0; j--)
				for (i = w - 1; i >= 0; i--) {
					label = (labels[j][i] * 20) + MASK;
					smDST.setSample(i, j, 0, (label & 0xff), dbDST);
					smDST.setSample(i, j, 1, (label & 0xff00) >> 8, dbDST);
					smDST.setSample(i, j, 2, (label & 0xff0000) >> 16, dbDST);
				}
		}
		else {
			for (j = h - 1, l = w * h - 1; j >= 0; j--)
				for (i = w - 1; i >= 0; i--) {
					if (labels[j][i] == 0)
						label = 0;
					else {
						label = (labels[j][i] * 20) + MASK;
					}
					smDST.setSample(i, j, 0, (label & 0xff), dbDST);
					smDST.setSample(i, j, 1, (label & 0xff00) >> 8, dbDST);
					smDST.setSample(i, j, 2, (label & 0xff0000) >> 16, dbDST);
				}
		}

		return dst;
	}

	/**
	 *
	 *
	 *
	 */

	public String toString() {
		return "Algorithme automatisé avec H-Minima ...";
	}

	public void stateChanged(ChangeEvent e) {
		JSlider source = (JSlider)e.getSource();
		int value = source.getValue();

		if (source == jsHmin)
			hmin = value;

		if (jcbAperçu.isSelected()) {
			traitement(src, dst);
			lbl.repaint();
		}	
	}

	public void actionPerformed(ActionEvent e) {
		JButton jb = (JButton)(e.getSource());

		if (jb == jbReset) {
			jsHmin.setValue(20);
		}
		else {
			if (jb == jbOK) {
				toutFaire = true;
				retPic = new Picture(name, traitement(src, dst));
				toutFaire = false;
			}
			else {
				retPic = null;
			}
			src = null;
			dst = null;
			jcbAperçu.setSelected(false);
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
		src = null;
		dst = null;
		retPic = null;
		jcbAperçu.setSelected(false);
		jdFiltre.setVisible(false);
	}
	public void windowDeactivated(WindowEvent e) {}
	public void windowDeiconified(WindowEvent e) {}
	public void windowIconified(WindowEvent e) {}
	public void windowOpened(WindowEvent e) {}
}