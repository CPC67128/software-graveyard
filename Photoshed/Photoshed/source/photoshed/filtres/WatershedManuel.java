package photoshed.filtres;

import java.awt.image.BufferedImage;
import java.awt.image.DataBuffer;
import java.awt.image.SampleModel;
import java.awt.image.WritableRaster;
import java.awt.image.ColorConvertOp;
import java.awt.Container;
import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.Graphics2D;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.awt.event.ItemListener;
import java.awt.event.ItemEvent;
import java.awt.event.MouseEvent;

import java.util.LinkedList;

import javax.swing.JRadioButton;
import javax.swing.JDialog;
import javax.swing.JButton;
import javax.swing.JSlider;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JLabel;
import javax.swing.ImageIcon;
import javax.swing.ButtonGroup;
import javax.swing.event.ChangeListener;
import javax.swing.event.ChangeEvent;
import javax.swing.event.MouseInputListener;

import photoshed.fenetres.Picture;
import photoshed.filtres.Filter;
import photoshed.filtres.StdFilter;
import photoshed.utils.Pile;
import photoshed.filtres.watershed.WS;

/**
 *
 *
 *
 */

public final class WatershedManuel extends StdFilter implements
				Filter, WindowListener,
				ActionListener, ChangeListener,
				ItemListener, MouseInputListener {

	private JSlider jsTaille;

	private JRadioButton jrbPaint;

	private JRadioButton jrbClear;

	private JRadioButton jrbRect;

	private JRadioButton jrbArc;

	private BufferedImage tmp;

	private int taille = 2;

	private final int STRT = 1;

	private int waterLabel = STRT;

	private boolean paint;

	private boolean rect;

	public final int MASK = 0x2f2f2f;

	public WatershedManuel() {

		super(false);
		addListener(this, null);

		lbl.addMouseListener(this);
		lbl.addMouseMotionListener(this);

		jsTaille = new JSlider(JSlider.HORIZONTAL, 1, 100, taille);
		jsTaille.addChangeListener(this);

		jrbPaint = new JRadioButton("Peindre", true);
		jrbPaint.addItemListener(this);

		jrbClear = new JRadioButton("Effacer", false);
		jrbClear.addItemListener(this);

		jrbRect = new JRadioButton("Rectangle", true);
		jrbRect.addItemListener(this);

		jrbArc = new JRadioButton("Cercle", false);
		jrbArc.addItemListener(this);

		ButtonGroup btnGrp = new ButtonGroup();
		btnGrp.add(jrbPaint);
		btnGrp.add(jrbClear);

		ButtonGroup jrbGrp = new ButtonGroup();
		jrbGrp.add(jrbRect);
		jrbGrp.add(jrbArc);

		jbReset.setText("Tout éffacer");

		paint = true;
		rect = true;
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
		tmp = new BufferedImage(w, h, BufferedImage.TYPE_4BYTE_ABGR);
		dst = new BufferedImage(w, h, BufferedImage.TYPE_3BYTE_BGR);
		
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

			JPanel top = new JPanel(new GridLayout(2, 3));
			top.add(jbOK);
			top.add(jbCancel);
			top.add(jbReset);
			top.add(jrbPaint);
			top.add(jrbClear);

			JPanel bottom = new JPanel(new GridLayout(2, 2));
			bottom.add(new JLabel("Taille : "));
			bottom.add(jsTaille);
			bottom.add(jrbRect);
			bottom.add(jrbArc);

			c.setLayout(new BorderLayout());
			c.add(top, BorderLayout.NORTH);
			c.add(bottom, BorderLayout.SOUTH);
			c.add(scroll, BorderLayout.CENTER);
			addWindowListener(this);
			firstTime = false;
		}
		jdFiltre.pack();
		jdFiltre.setLocationRelativeTo(pic);

		Graphics2D gtmp = tmp.createGraphics();
		gtmp.drawImage(src, 0, 0, jdFiltre);
		gtmp.dispose();
		gtmp = dst.createGraphics();
		gtmp.drawImage(src, 0, 0, jdFiltre);
		gtmp.dispose();
		lbl.repaint();
		jdFiltre.setVisible(true);
		if (retPic != null)
			retPic.property = new WS((BufferedImage)pic.property, pic.getSource(), waterLabel);

		waterLabel = STRT;

		dst = null;
		src = null;
		tmp = null;

		return retPic;
	}

	/**
	 *
	 *
	 *
	 */

	public BufferedImage traitement(BufferedImage src, BufferedImage dst) {

		int w = src.getWidth(),
			h = src.getHeight();

		int i,
			j,
			k,
			l,
			g,
			x,
			y,
			yk,
			label;

		SampleModel smSRC, smDST, smTMP;
		WritableRaster wrSRC, wrDST, wrTMP;
		DataBuffer dbSRC, dbDST, dbTMP;

		wrSRC = src.getRaster();
		wrTMP = tmp.getRaster();
		wrDST = dst.getRaster();

		smSRC = wrSRC.getSampleModel();
		smTMP = wrTMP.getSampleModel();
		smDST = wrDST.getSampleModel();

		dbSRC = wrSRC.getDataBuffer();
		dbTMP = wrTMP.getDataBuffer();
		dbDST = wrDST.getDataBuffer();
		
		int [] source = new int[w * h];
		int [][] tab = new int[h][w];
		int [][] labels = new int[h][w];

		smTMP.getSamples(0, 0, w, h, 0, source, dbTMP);
		for (j = h - 1, l = w * h - 1; j >= 0; j--)
			for (i = w - 1; i >= 0; i--, l--) {
				labels[j][i] = source[l];
			}
		smTMP.getSamples(0, 0, w, h, 1, source, dbTMP);
		for (j = h - 1, l = w * h - 1; j >= 0; j--)
			for (i = w - 1; i >= 0; i--, l--) {
				labels[j][i] |= source[l] << 8;
			}
		smTMP.getSamples(0, 0, w, h, 2, source, dbTMP);
		for (j = h - 1, l = w * h - 1; j >= 0; j--)
			for (i = w - 1; i >= 0; i--, l--) {
				labels[j][i] |= source[l] << 16;
			}

		smSRC.getSamples(0, 0, w, h, 0, source, dbSRC);

		for (j = h - 1, l = w * h - 1; j >= 0; j--)
			for (i = w - 1; i >= 0; i--, l--) {
				tab[j][i] = source[l] | source[l] << 8 | source[l] << 16;
			}

		Point p = null;
			
		Pile [] niveau = new Pile[256];

		for (i = 255; i >= 0; i--)
			niveau[i] = new Pile();

		for (j = h - 1, l = (w * h) - 1; j >= 0; j--)
			for (i = w - 1 ; i >= 0 ; i--, l--)
				if (labels[j][i] != tab[j][i])
					niveau[source[l]].push((Object)new Point(i, j));
				else
					labels[j][i] = 0;

		for (g = 0; g < 256; g++) {
			while (!niveau[g].isEmpty()) {
				p = (Point)niveau[g].pop();
				x = p.x;
				y = p.y;
				label = labels[y][x];
				k = y * w + x;
				if (x>0 && labels[y][x-1]==0) {
					labels[y][x-1] = label;
					yk = k - 1;
					l = (source[yk] > g)?(source[yk]):(g);
					niveau[l].push((Object)new Point(x-1, y));
				}

				if (x<w-1 && labels[y][x+1]==0) {
					labels[y][x+1] = label;
					yk = k + 1;
					l = (source[yk] > g)?(source[yk]):(g);
					niveau[l].push((Object)new Point(x+1, y));
				}

				if (y>0 && labels[y-1][x]==0) {
					labels[y-1][x] = label;
					yk = k - w;
					l = (source[yk] > g)?(source[yk]):(g);
					niveau[l].push((Object)new Point(x, y-1));
				}

				if (y<h-1 && labels[y+1][x]==0) {
					labels[y+1][x] = label;
					yk = k + w;
					l = (source[yk] > g)?(source[yk]):(g);
					niveau[l].push((Object)new Point(x, y+1));
				}
			}
			niveau[g] = null;
		}

		for (j = h - 1, l = w * h - 1; j >= 0; j--)
			for (i = w - 1; i >= 0; i--) {
				smDST.setSample(i, j, 0, (labels[j][i] & 0xff), dbDST);
				smDST.setSample(i, j, 1, (labels[j][i] & 0xff00) >> 8, dbDST);
				smDST.setSample(i, j, 2, (labels[j][i] & 0xff0000) >> 16, dbDST);
			}

		return dst;
	}

	/**
	 *
	 *
	 *
	 */

	public String toString() {
		return "Algorithme manuel ...";
	}

	public void stateChanged(ChangeEvent e) {
		JSlider source = (JSlider)e.getSource();
		int value = source.getValue();

		if (source == jsTaille)
			taille = value;
	}

	public void actionPerformed(ActionEvent e) {
		JButton jb = (JButton)(e.getSource());
		if (jb == jbReset) {
			Graphics2D gtmp = dst.createGraphics();
			gtmp.drawImage(src, 0, 0, jdFiltre);
			waterLabel = STRT;
			gtmp.dispose();
			gtmp = tmp.createGraphics();
			gtmp.drawImage(src, 0, 0, jdFiltre);
			gtmp.dispose();
			lbl.repaint();
		}
		else {
			if (jb == jbOK) {
				retPic = new Picture(name, traitement(src, dst));
			}
			else {
				retPic = null;
			}
			src = null;
			dst = null;
			tmp = null;
			jdFiltre.setVisible(false);
		}
	}

	public void itemStateChanged(ItemEvent e) {
		JRadioButton jrb = (JRadioButton)e.getSource();
		
		if (jrbPaint == jrb) {
			paint = true;
		}
		else if (jrbClear == jrb) {
			paint = false;
		}
		else if (jrb == jrbRect) {
			rect = true;
		}
		else {
			rect = false;
		}
	}

	public void mouseClicked(MouseEvent e) {}
	public void mouseEntered(MouseEvent e) {}
	public void mouseExited(MouseEvent e) {}
	public void mousePressed(MouseEvent e) {
		Graphics2D dstG = dst.createGraphics();
		Graphics tmpG = tmp.getGraphics();
		int xtmp = lbl.getWidth() - dst.getWidth();
		int ytmp = lbl.getHeight() - dst.getHeight();
		if (xtmp < 0)
			xtmp = 0;
		if (ytmp < 0)
			ytmp = 0;
		if (paint) {
			int c = (waterLabel * 20) + MASK;
			c = ((c & 0xff0000) >> 16) | (c & 0xff00) | ((c & 0xff) << 16);
			tmpG.setColor(new java.awt.Color(c));
		}
		else
			tmpG.setColor(new java.awt.Color(0,0,0,0));
		if (rect)
			tmpG.fillRect(e.getX() - ((taille + xtmp) / 2), e.getY() - ((taille + ytmp) / 2), taille, taille);
		else
			tmpG.fillArc(e.getX() - ((taille + xtmp) / 2), e.getY() - ((taille + ytmp) / 2), taille, taille, 0, 360);
		tmpG.dispose();
		if (dstG.drawImage(tmp, 0, 0, jdFiltre))
			lbl.repaint();
		dstG.dispose();
	}
	public void mouseReleased(MouseEvent e) {
		waterLabel++;
	}
	public void mouseDragged(MouseEvent e) {
		Graphics2D gtmp = dst.createGraphics();
		Graphics tmpG = tmp.getGraphics();

		int xtmp = lbl.getWidth() - dst.getWidth();
		int ytmp = lbl.getHeight() - dst.getHeight();
		if (xtmp < 0)
			xtmp = 0;
		if (ytmp < 0)
			ytmp = 0;
		if (paint){
			int c = (waterLabel * 20) + MASK;
			c = ((c & 0xff0000) >> 16) | (c & 0xff00) | ((c & 0xff) << 16);
			tmpG.setColor(new java.awt.Color(c));
		}
		else
			tmpG.setColor(new java.awt.Color(0,0,0,0));
		if (rect)
			tmpG.fillRect(e.getX() - ((taille + xtmp) / 2), e.getY() - ((taille + ytmp) / 2), taille, taille);
		else
			tmpG.fillArc(e.getX() - ((taille + xtmp) / 2), e.getY() - ((taille + ytmp) / 2), taille, taille, 0, 360);

		/*if (paint) {
			tmpG.setColor(new java.awt.Color((waterLabel * 20) + MASK));
			if (rect)
				tmpG.fillRect(e.getX() - ((taille + xtmp) / 2), e.getY() - ((taille + ytmp) / 2), taille, taille);
			else
				tmpG.fillArc(e.getX() - ((taille + xtmp) / 2), e.getY() - ((taille + ytmp) / 2), taille, taille, 0, 360);
		}
		else {
			tmpG.setColor(new java.awt.Color(0,255,0,0));
			gtmp.drawImage(src, 0, 0, jdFiltre);
			if (rect)
				tmpG.fillRect(e.getX() - ((taille + xtmp) / 2), e.getY() - ((taille + ytmp) / 2), taille, taille);
			else
				tmpG.fillArc(e.getX() - ((taille + xtmp) / 2), e.getY() - ((taille + ytmp) / 2), taille, taille, 0, 360);
			tmpG.dispose();
			gtmp.drawImage(tmp, 0, 0, jdFiltre);
			gtmp.dispose();
			tmpG = tmp.getGraphics();
			//tmpG.drawImage(dst, 0, 0, jdFiltre);
			gtmp = dst.createGraphics();
		}*/

		tmpG.dispose();

		if (gtmp.drawImage(tmp, 0, 0, jdFiltre)) {
			if (paint)
				gtmp.setColor(new java.awt.Color(255,0,0));
			else
				gtmp.setColor(new java.awt.Color(255,255,255));
			if (rect)
				gtmp.drawRect(e.getX() - ((taille + xtmp) / 2), e.getY() - ((taille + ytmp)/ 2), taille, taille);
			else
				gtmp.drawArc(e.getX() - ((taille + xtmp) / 2), e.getY() - ((taille + ytmp) / 2), taille, taille, 0, 360);
			lbl.repaint();
		}
		gtmp.dispose();
	}
	public void mouseMoved(MouseEvent e) {
		Graphics2D gtmp = dst.createGraphics();
		
		if (gtmp.drawImage(tmp, 0, 0, jdFiltre)) {
			int xtmp = lbl.getWidth() - dst.getWidth();
			int ytmp = lbl.getHeight() - dst.getHeight();
			if (xtmp < 0)
				xtmp = 0;
			if (ytmp < 0)
				ytmp = 0;
			if (paint) {
				gtmp.setColor(new java.awt.Color(255,0,0));
				if (rect)
					gtmp.drawRect(e.getX() - ((taille + xtmp) / 2), e.getY() - ((taille + ytmp)/ 2), taille, taille);
				else
					gtmp.drawArc(e.getX() - ((taille + xtmp) / 2), e.getY() - ((taille + ytmp) / 2), taille, taille, 0, 360);
			}
			else {
				gtmp.setColor(new java.awt.Color(255, 255, 255));
				if (rect)
					gtmp.fillRect(e.getX() - ((taille + xtmp) / 2), e.getY() - ((taille + ytmp)/ 2), taille, taille);
				else
					gtmp.fillArc(e.getX() - ((taille + xtmp) / 2), e.getY() - ((taille + ytmp) / 2), taille, taille, 0, 360);
			}
			lbl.repaint();
		}

		gtmp.dispose();
	}

	public void windowActivated(WindowEvent e) {}
	public void windowClosed(WindowEvent e) {}
	public void windowClosing(WindowEvent e) {
		src = null;
		dst = null;
		tmp = null;
		retPic = null;
		jdFiltre.setVisible(false);
	}
	public void windowDeactivated(WindowEvent e) {}
	public void windowDeiconified(WindowEvent e) {}
	public void windowIconified(WindowEvent e) {}
	public void windowOpened(WindowEvent e) {}

}