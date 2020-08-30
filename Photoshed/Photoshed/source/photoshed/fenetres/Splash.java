package photoshed.fenetres;

import java.awt.Window;
import java.awt.Frame;
import java.awt.Color;
import java.awt.Canvas;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Dimension;
import java.awt.Toolkit;

import javax.swing.ImageIcon;

public final class Splash extends Window {

	private Image img = null;

	private Canvas cvs = new Canvas();

	private String txt = "";

	public Splash(Frame w) {
		super(w);
		setLayout(null);
		ImageIcon imgTmp = new ImageIcon("Logo.jpg");
		img = imgTmp.getImage();

		Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
		setLocation(screen.width/2-(imgTmp.getIconWidth()/2), screen.height/2-(imgTmp.getIconHeight()/2));

		setBackground(Color.gray);
		setSize(imgTmp.getIconWidth(), imgTmp.getIconHeight());
		setVisible(true);
	}

	public void paint(Graphics g) {
		g.drawImage(img, 0, 0 , getWidth(), getHeight(), cvs);
		g.setColor(Color.black);
		g.drawString(txt, 115, 260);
		
	}

	public void update(Graphics g) {
		paint(g);
	}

	public void repaint() {
		update(getGraphics());
	}

	public void setText(String t) {
		txt = t;
		repaint();
		long time = System.currentTimeMillis();
		while(System.currentTimeMillis() - time < 2);
	}

	public void dispose() {
		super.dispose();
		System.gc();
	}
}