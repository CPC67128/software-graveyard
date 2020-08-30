package photoshed.fenetres;

import javax.swing.JInternalFrame;
import javax.swing.JScrollPane;
import javax.swing.JLabel;
import javax.swing.ImageIcon;
import javax.swing.event.InternalFrameListener;
import javax.swing.event.InternalFrameEvent;

import java.awt.image.BufferedImage;
import java.awt.BorderLayout;
import java.awt.event.MouseListener;

public final class Picture extends JInternalFrame implements InternalFrameListener {

	private BufferedImage src = null;

	private ImageIcon icon = null;

	private JLabel lbl = null;

	private JScrollPane scroll;

	private String name = null;

	public Object property = null;

	public Picture(String nom, BufferedImage img) {
		super(nom, true, true, true, true);

		name = nom;

		src = img;

		icon = new ImageIcon(src);
		lbl = new JLabel(icon);
		lbl.setVisible(true);

		scroll = new JScrollPane();
		scroll.getViewport().add(lbl);
		scroll.setVisible(true);

		setContentPane(scroll);
		addInternalFrameListener(this);
		setDefaultCloseOperation(javax.swing.WindowConstants.DO_NOTHING_ON_CLOSE);
		pack();
		setVisible(true);
	}

	public BufferedImage getSource() {
		return src;
	}

	public void setSource(BufferedImage img) {
		src = img;
		icon.setImage(src);
	}

	public String getName() {
		return name;
	}

	public void internalFrameClosing(InternalFrameEvent e) {
		destroye();
	}

	public void destroye() {
		property = null;
		name = null;
		scroll = null;
		lbl = null;
		icon = null;
		src = null;
		try {
			getDesktopPane().getDesktopManager().closeFrame(this);
			dispose();
		}
		catch(NullPointerException e) {}
	}

	public void addPopupMenu(MouseListener ml) {
		lbl.addMouseListener(ml);
	}

	public void internalFrameDeactivated(InternalFrameEvent e) {}
	public void internalFrameDeiconified(InternalFrameEvent e) {}
	public void internalFrameIconified(InternalFrameEvent e) {}
	public void internalFrameOpened(InternalFrameEvent e) {}
	public void internalFrameActivated(InternalFrameEvent e) {}
	public void internalFrameClosed(InternalFrameEvent e) {}
}