package photoshed.filtres;

import java.awt.image.BufferedImage;
import java.awt.image.DataBuffer;
import java.awt.image.SampleModel;
import java.awt.image.WritableRaster;
import java.awt.Graphics2D;
import java.awt.GridLayout;
import java.awt.Rectangle;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JDialog;
import javax.swing.JScrollPane;
import javax.swing.JButton;
import javax.swing.event.MouseInputListener;

import photoshed.fenetres.Picture;
import photoshed.filtres.Filter;
import photoshed.filtres.StdFilter;

public final class Crop extends StdFilter implements
			Filter, WindowListener,
			MouseInputListener, ActionListener {

	private Rectangle selection;

	public Crop() {
		super(false);

		addListener(this, null);

		lbl.addMouseListener(this);
		lbl.addMouseMotionListener(this);

		jbReset.setText("Déselectionner");
		selection = null;
	}

	/**
	 *
	 *
	 *
	 */

	public boolean isApplicable(Picture pic){
		return true;
	}


	public Picture execute(Picture pic){
		if (pic == null)
			return null;

		src = pic.getSource();
		dst = new BufferedImage(src.getWidth(), src.getHeight(),  BufferedImage.TYPE_INT_RGB);
System.out.println(src.getType());
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

			JPanel top = new JPanel(new GridLayout(1, 3));
			top.add(jbOK);
			top.add(jbCancel);
			top.add(jbReset);

			c.setLayout(new BorderLayout());
			c.add(top, BorderLayout.NORTH);
			c.add(scroll, BorderLayout.CENTER);
			addWindowListener(this);
			firstTime = false;
		}
		jdFiltre.pack();
		jdFiltre.setLocationRelativeTo(pic);

		selection = new Rectangle(0, 0, src.getWidth(), src.getHeight());

		Graphics2D gtmp = dst.createGraphics();
		gtmp.drawImage(src, 0, 0, jdFiltre);
		gtmp.dispose();
		lbl.repaint();
		jdFiltre.setVisible(true);
		if (retPic != null)
			retPic.property = pic.property;
		dst = null;
		src = null;
		selection = null;

		return retPic;
	}

	public BufferedImage traitement(BufferedImage src, BufferedImage dst) {

		BufferedImage ret = new BufferedImage(selection.width, selection.height, src.getType());

		SampleModel smSRC, smRET;
		WritableRaster wrSRC, wrRET;
		DataBuffer dbSRC, dbRET;
		
		wrSRC = src.getRaster();
		wrRET = ret.getRaster();
		smSRC = wrSRC.getSampleModel();
		smRET = wrRET.getSampleModel();
		dbSRC = wrSRC.getDataBuffer();
		dbRET = wrRET.getDataBuffer();
		
		int w = src.getWidth();
		int h = src.getHeight();

		for (int i = smSRC.getNumBands() - 1; i >= 0; i--) {
			smRET.setSamples(0, 0, selection.width, selection.height, i, smSRC.getSamples(selection.x, selection.y, selection.width, selection.height, i, (int[]) null, dbSRC), dbRET);
		}

		return ret;

	}

	public String toString() {
		return "Recadrer ...";
	}

	public void actionPerformed(ActionEvent e) {
		JButton jb = (JButton)(e.getSource());
		if (jb == jbReset) {
			Graphics2D gtmp = dst.createGraphics();
			gtmp.drawImage(src, 0, 0, jdFiltre);
			gtmp.dispose();
			lbl.repaint();
			selection.setBounds(0, 0, src.getWidth(), src.getHeight());
			
		}
		else {
			if (jb == jbOK) {
				retPic = new Picture(name, traitement(src, null));
			}
			else {
				retPic = null;
			}
			jdFiltre.setVisible(false);
		}
	}

	public void mouseClicked(MouseEvent e) {
		Graphics2D gtmp = dst.createGraphics();
		gtmp.drawImage(src, 0, 0, jdFiltre);
		gtmp.dispose();
		lbl.repaint();
		selection.setBounds(0, 0, src.getWidth(), src.getHeight());
	}
	public void mouseEntered(MouseEvent e) {}
	public void mouseExited(MouseEvent e) {}
	public void mousePressed(MouseEvent e) {
		int xtmp = lbl.getWidth() - src.getWidth();
		int ytmp = lbl.getHeight() - src.getHeight();
		if (xtmp < 0)
			xtmp = 0;
		if (ytmp < 0)
			ytmp = 0;
		selection.setLocation(e.getX() - (xtmp >> 1), e.getY() - (ytmp >> 1));
		selection.setSize(0, 0);
	}

	public void mouseReleased(MouseEvent e) {
		/*Graphics2D dstG = dst.createGraphics();
		int xtmp = lbl.getWidth() - src.getWidth();
		int ytmp = lbl.getHeight() - src.getHeight();
		if (xtmp < 0)
			xtmp = 0;
		if (ytmp < 0)
			ytmp = 0;
		if (dstG.drawImage(src, 0, 0, jdFiltre)) {
			dstG.setColor(Color.green);
			int x = e.getX() - (xtmp >> 1),
				y = e.getY() - (ytmp >> 1),
				width = x - selection.x,
				height = y - selection.y;

			if (width <= 0) {
				selection.width -= width;
				selection.x = y;
			}
			else {
				selection.width = width;
			}
			if (height <= 0) {
				selection.height -= height;
				selection.y = y;
			}
			else {
				selection.height = height;
			}

			dstG.drawRect(selection.x, selection.y, selection.width, selection.height);
			lbl.repaint();
		}
		dstG.dispose();*/
	}
		
	public void mouseDragged(MouseEvent e) {
		Graphics2D dstG = dst.createGraphics();
		int xtmp = lbl.getWidth() - src.getWidth();
		int ytmp = lbl.getHeight() - src.getHeight();
		if (xtmp < 0)
			xtmp = 0;
		if (ytmp < 0)
			ytmp = 0;
		if (dstG.drawImage(src, 0, 0, jdFiltre)) {
			dstG.setColor(Color.green);
			int x = e.getX() - (xtmp >> 1),
				y = e.getY() - (ytmp >> 1),
				width = x - selection.x,
				height = y - selection.y;

			if (width < 0 && x >= 0) {
				selection.width -= width;
				selection.x = x;
			}
			else if (width > 0) {
				selection.width = width;
			}
			if (height < 0 && y >= 0) {
				selection.height -= height;
				selection.y = y;
			}
			else if (height > 0) {
				selection.height = height;
			}

			if (selection.x < 0)
				selection.x = 0;
			if (selection.y < 0)
				selection.y = 0;
			if (selection.width + selection.x >= src.getWidth())
				selection.width = src.getWidth() - selection.x - 1;
			if (selection.height + selection.y >= src.getHeight())
				selection.height = src.getHeight() - selection.y  - 1;
			dstG.drawRect(selection.x, selection.y, selection.width, selection.height);
			lbl.repaint();
		}
		dstG.dispose();
	}

	public void mouseMoved(MouseEvent e) {
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