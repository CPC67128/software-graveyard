package photoshed.filtres;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.FlowLayout;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Rectangle;
import java.awt.Point;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.awt.image.AffineTransformOp;
import java.awt.image.ColorConvertOp;
import java.awt.image.DataBuffer;
import java.awt.image.SampleModel;
import java.awt.image.WritableRaster;
import java.awt.event.MouseEvent;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.awt.event.ItemListener;
import java.awt.event.ItemEvent;
import java.awt.event.AdjustmentEvent;
import java.awt.event.AdjustmentListener;
import java.awt.geom.AffineTransform;

import javax.swing.JPanel;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JDialog;
import javax.swing.JButton;
import javax.swing.JSlider;
import javax.swing.JCheckBox;
import javax.swing.JRadioButton;
import javax.swing.ImageIcon;
import javax.swing.JScrollPane;
import javax.swing.JScrollBar;
import javax.swing.JComponent;
import javax.swing.JFileChooser;
import javax.swing.ButtonGroup;
import javax.swing.event.MouseInputListener;
import javax.swing.event.ChangeListener;
import javax.swing.event.ChangeEvent;
import javax.swing.border.EmptyBorder;

import java.io.FileOutputStream;
import java.io.DataOutputStream;

import java.util.Hashtable;
import java.util.Enumeration;

import photoshed.fenetres.Picture;
import photoshed.fenetres.Erreur;
import photoshed.filtres.Filter;
import photoshed.filtres.watershed.*;

public final class WatershedStat extends JComponent implements
	Filter, ItemListener, ChangeListener,
	ActionListener, WindowListener, MouseInputListener,
	AdjustmentListener {

	private static final String ZOOM = "Zoom : x ";

	private static final String AIRE = "Aire (en pixel) : ";

	private static final String CTR1 = "La region à pour centre : X = ";

	private static final String CTR2 = ";Y = ";

	private static final String NOAIRE = "Cliquez sur une zone d'une des deux images pour";

	private static final String NOCENTRE = "avoir l'aire, la position (le centre) et le";

	private static final String NOMOYEN = "niveaux de gris moyen de la region selectionnée";

	private JButton quit;

	private JButton reset;

	private JButton save;

	private JLabel lblNbRegs;

	private JLabel txtNbRegs;

	private JCheckBox select;

	private JCheckBox trace;

	private JRadioButton rGdt;

	private JRadioButton rSrc;

	private BufferedImage iWater;

	private BufferedImage iSelec;

	private BufferedImage iTmp;

	private JLabel lblWater;

	private JLabel lblSelec;

	private ImageIcon icoWater;

	private ImageIcon icoSelec;

	private JScrollPane scrollWater;

	private JScrollPane scrollSelec;

	private JLabel aire;

	private JLabel centre;

	private JLabel moyen;

	private JLabel lblZoom;

	private JSlider sliZoom;

	private JDialog dialog;

	private WSStat wss;

	private WSRegion wsr;

	private WS ws;

	private Picture retPic;

	private boolean scrolling;

	private double zoom;

	public WatershedStat() {
		setLayout(new BorderLayout());
		setVisible(false);

		JPanel north = new JPanel(new GridLayout(0, 1)),
				center = new JPanel(new GridLayout(1, 2)),
				south = new JPanel(new GridLayout(0, 1)),
				jp;

		lblWater = new JLabel();
		lblWater.addMouseListener(this);
		lblWater.addMouseMotionListener(this);
		lblSelec = new JLabel();
		lblSelec.addMouseListener(this);
		lblSelec.addMouseMotionListener(this);

		scrollWater = new JScrollPane();
		scrollWater.getViewport().add(lblWater);
		scrollWater.getVerticalScrollBar().addAdjustmentListener(this);
		scrollWater.getHorizontalScrollBar().addAdjustmentListener(this);
		scrollSelec = new JScrollPane();
		scrollSelec.getViewport().add(lblSelec);
		scrollSelec.getVerticalScrollBar().addAdjustmentListener(this);
		scrollSelec.getHorizontalScrollBar().addAdjustmentListener(this);

		quit = new JButton("Quitter") ;
		quit.addActionListener(this);
		reset = new JButton("Réinitialiser") ;
		reset.addActionListener(this);
		save = new JButton("Créer un rapport");
		save.addActionListener(this);
		jp = new JPanel(new FlowLayout());
		jp.add(quit);
		jp.add(reset);
		jp.add(save);
		north.add(jp);

		lblNbRegs = new JLabel("Nombre de région : ");
		txtNbRegs = new JLabel();
		jp = new JPanel(new FlowLayout());
		jp.add(lblNbRegs);
		jp.add(txtNbRegs);
		north.add(jp);

		select = new JCheckBox("Afficher uniquement la selection");
		select.setSelected(false);
		select.addItemListener(this);
		trace = new JCheckBox("Tracer les contoures");
		trace.setSelected(false);
		trace.addItemListener(this);
		trace.setVisible(false);
		jp = new JPanel(new FlowLayout());
		jp.add(select);
		jp.add(trace);
		north.add(jp);

		ButtonGroup bg = new ButtonGroup();
		rGdt = new JRadioButton("Gradient");
		rGdt.setSelected(true);
		rGdt.addItemListener(this);
		rSrc = new JRadioButton("Source");
		rSrc.setSelected(false);
		rSrc.addItemListener(this);
		bg.add(rGdt);
		bg.add(rSrc);
		jp = new JPanel(new FlowLayout());
		jp.add(rGdt);
		jp.add(rSrc);
		north.add(jp);

		aire = new JLabel(NOAIRE);
		jp = new JPanel(new FlowLayout());
		jp.add(aire);
		south.add(jp);

		centre = new JLabel(NOCENTRE);

		moyen = new JLabel(NOMOYEN);
		jp = new JPanel(new FlowLayout());
		jp.add(centre);
		jp.add(moyen);
		south.add(jp);

		lblZoom = new JLabel(ZOOM + "1.0");
		lblZoom.setAlignmentX(Component.CENTER_ALIGNMENT);
		lblZoom.setAlignmentY(Component.CENTER_ALIGNMENT);
		sliZoom = new JSlider(javax.swing.SwingConstants.HORIZONTAL, -6, 20, 0);
		sliZoom.addChangeListener(this);
		jp = new JPanel(new FlowLayout());
		jp.add(lblZoom);
		jp.add(sliZoom);
		south.add(jp);

		center.add(scrollWater);
		center.add(scrollSelec);

		add(north, BorderLayout.NORTH);
		add(center, BorderLayout.CENTER);
		add(south, BorderLayout.SOUTH);

		dialog = null;
		retPic = null;
		scrolling = false;
		zoom = 1.0;
	}

	public boolean isApplicable(Picture pic) {
		return pic.property instanceof WS;
	}

	public Picture execute(Picture pic) {
		ws = (WS)pic.property;
		construction(pic.getSource(), ws.getSource(), ws.getNbRegions());
		txtNbRegs.setText("" + (ws.getNbRegions() - 1));
		iWater = pic.getSource();
		iSelec = rGdt.isSelected()?ws.getGradient():ws.getSource();
		icoWater = new ImageIcon(iWater);
		icoSelec = new ImageIcon(iSelec);
		sliZoom.setValue(0);
		lblWater.setIcon(icoWater);
		lblSelec.setIcon(icoSelec);
		setVisible(true);

		Component ctmp = pic.getDesktopPane();
		while(!(ctmp instanceof JFrame)) {
			ctmp = ctmp.getParent();
		}

		dialog = new JDialog((JFrame)(ctmp), toString(), true);
		dialog.setDefaultCloseOperation(javax.swing.WindowConstants.DO_NOTHING_ON_CLOSE);
		dialog.addWindowListener(this);
		dialog.setContentPane(this);
		dialog.pack();
        dialog.setLocationRelativeTo(ctmp);
		dialog.show();

		wsr = null;
		ws = null;
		wss.clear();
		wss = null;
		iWater = null;
		iSelec = null;
		lblWater.setIcon(null);
		lblSelec.setIcon(null);
		setVisible(false);
		aire.setText(NOAIRE);
		centre.setText(NOCENTRE);
		moyen.setText(NOMOYEN);

		return null;
	}

	public BufferedImage traitement(BufferedImage src, BufferedImage dst) {
		if (rGdt.isSelected())
			iSelec = ws.getGradient();
		else
			iSelec = ws.getSource();

		BufferedImage tmp;

		AffineTransform at = new AffineTransform();
		at.scale(zoom, zoom);
		AffineTransformOp scale = new AffineTransformOp(at, AffineTransformOp.TYPE_NEAREST_NEIGHBOR);

		if (wsr != null) {

			tmp = new BufferedImage(iSelec.getWidth(), iSelec.getHeight(), iSelec.getType());

			SampleModel smSRC, smRET;
			WritableRaster wrSRC, wrRET;
			DataBuffer dbSRC, dbRET;
		
			wrSRC = iSelec.getRaster();
			wrRET = tmp.getRaster();
			smSRC = wrSRC.getSampleModel();
			smRET = wrRET.getSampleModel();
			dbSRC = wrSRC.getDataBuffer();
			dbRET = wrRET.getDataBuffer();
			
			Graphics2D graph = tmp.createGraphics();

			WSPoint wsp = null;
			if (select.isSelected()) {

				for (Enumeration enum = wsr.getWSPoint(); enum.hasMoreElements();) {
					wsp = (WSPoint)enum.nextElement();
					/*tmp.setRGB(wsp.x, wsp.y, iSelec.getRGB(wsp.x, wsp.y));*/
					for (int i = smSRC.getNumBands() - 1; i >= 0; i--)
						smRET.setSamples(wsp.x, wsp.y, 1, 1, i, smSRC.getSamples(wsp.x, wsp.y, 1, 1, i, (int [])null, dbSRC), dbRET);
				}
			}
			else
				graph.drawImage(iSelec, 0, 0, lblSelec);

			if (trace.isSelected()) {
				wrSRC = iWater.getRaster();
				smSRC = wrSRC.getSampleModel();
				dbSRC = wrSRC.getDataBuffer();

				Rectangle rec = wsr.getBounds();

				int [][] labels = new int [rec.width][rec.height];
				for (int i = rec.height - 1; i >= 0; i--) {
					for (int j = rec.width - 1; j >= 0; j--) {
						wsp = wsr.rechercherWSPointA(rec.x + j, rec.y + i);
						if (wsp != null) {
							//if (
						}
					}
				}
			}

			graph.dispose();
		}
		else
			tmp = iSelec;

		if (src != null) {
			icoWater.setImage(scale.filter(iWater, null));
			lblWater.revalidate();
			lblWater.repaint();
		}
		if (dst != null) {
			icoSelec.setImage(scale.filter(tmp, null));
			lblSelec.revalidate();
			lblSelec.repaint();
		}

		return null;
	}

	public void windowActivated(WindowEvent e) {}
	public void windowClosed(WindowEvent e) {}
	public void windowClosing(WindowEvent e) {
		dialog.remove(this);
		dialog.dispose();
		dialog = null;
	}
	public void windowDeactivated(WindowEvent e) {}
	public void windowDeiconified(WindowEvent e) {}
	public void windowIconified(WindowEvent e) {}
	public void windowOpened(WindowEvent e) {}
	
	public void actionPerformed(ActionEvent e) {
		JButton jb = (JButton)(e.getSource());
		if (jb == reset) {
			wsr = null;
			select.setSelected(false);
			trace.setSelected(false);
			rGdt.setSelected(true);
			sliZoom.setValue(0);
			traitement(iWater, iSelec);
		}
		else if (jb == save) {
			JFileChooser jfc = new JFileChooser();
			jfc.setFileSelectionMode(JFileChooser.FILES_ONLY);
			jfc.setMultiSelectionEnabled(false);
			int valide = jfc.showSaveDialog(this);

			if (valide == JFileChooser.APPROVE_OPTION) {
				try {
					DataOutputStream dos = new DataOutputStream(new FileOutputStream(jfc.getSelectedFile()));
					dos.writeBytes(wss.toString());
					dos.flush();
					dos.close();
				}
				catch (Exception ex) {
					Erreur.print(ex, "");
				}
			}
		}
		else {
			dialog.remove(this);
			dialog.dispose();
			dialog = null;
		}
	}

	public void mouseClicked(MouseEvent e) {
	}
	public void mouseEntered(MouseEvent e) {}
	public void mouseExited(MouseEvent e) {}
	public void mousePressed(MouseEvent e) {}
	public void mouseReleased(MouseEvent e) {
		JLabel lbl = (JLabel)e.getSource();

		SampleModel sm;
		WritableRaster wr;
		DataBuffer db;

		int x = e.getX();
		int y = e.getY();
		int ytmp = lbl.getHeight() - ((BufferedImage)icoWater.getImage()).getHeight();

		if (ytmp > 0)
			y -= (ytmp/2);
		if (zoom != 1) {
			x /= zoom;
			y /= zoom;
		}

		wr = iWater.getRaster();
		sm = wr.getSampleModel();
		db = wr.getDataBuffer();

		wsr = wss.rechercherWSRegion((sm.getSample(x, y, 2, db) & 0xff) << 16 |
									(sm.getSample(x, y, 1, db) & 0xff) << 8 |
									(sm.getSample(x, y, 0, db) & 0xff));
		traitement(null, iSelec);

		if (wsr != null) {
			int a = wsr.aire();
			aire.setText(AIRE + a + " (= " + ((a * 100.0F) / (iWater.getWidth() * iWater.getHeight())) + "% de l'image)");
			Point p = wsr.centre();
			centre.setText(CTR1 + p.x + CTR2 + p.y);
			moyen.setText("Niveau de gris moyen : " + wsr.moyenneDeCouleur());
		}
		else {
			aire.setText(NOAIRE);
			centre.setText(NOCENTRE);
			moyen.setText(NOMOYEN);
		}
	}

	public void mouseDragged(MouseEvent e) {
		JLabel lbl = (JLabel)e.getSource();

		int x = e.getX();
		int y = e.getY();
		int ytmp = lbl.getHeight() - ((BufferedImage)icoWater.getImage()).getHeight();

		if (ytmp > 0)
			y -= (ytmp/2);
		if (zoom != 1) {
			x /= zoom;
			y /= zoom;
		}
		dialog.setTitle(toString() + "   x : " + x + " |  y : " + y);
	}

	public void mouseMoved(MouseEvent e) {
		JLabel lbl = (JLabel)e.getSource();

		int x = e.getX();
		int y = e.getY();
		int ytmp = lbl.getHeight() - ((BufferedImage)icoWater.getImage()).getHeight();

		if (ytmp > 0)
			y -= (ytmp/2);
		if (zoom != 1) {
			x /= zoom;
			y /= zoom;
		}
		dialog.setTitle(toString() + "   x : " + x + " |  y : " + y);
	}

	public void stateChanged(ChangeEvent e) {
		zoom = sliZoom.getValue() / 10.0;

		if (zoom < 0)
			zoom = 1 + zoom;
		else
			zoom++;

		lblZoom.setText(ZOOM + zoom);

		traitement(iWater, iSelec);
	}

	public void itemStateChanged(ItemEvent e) {
		traitement(null, iSelec);
	}

	public void adjustmentValueChanged(AdjustmentEvent e) {
		if (!scrolling) {
			scrolling = true;
			JScrollBar jsb = (JScrollBar)e.getSource(),
						tmp;

			if (jsb == scrollWater.getVerticalScrollBar())
				tmp = scrollSelec.getVerticalScrollBar();
			else if (jsb == scrollSelec.getVerticalScrollBar())
				tmp = scrollWater.getVerticalScrollBar();
			else if (jsb == scrollWater.getHorizontalScrollBar())
				tmp = scrollSelec.getHorizontalScrollBar();
			else
				tmp = scrollWater.getHorizontalScrollBar();

			tmp.setValue(jsb.getValue());
			scrolling = false;
		}
	}

	private void construction(BufferedImage watershed, BufferedImage source, int nbRegions) {

		int w = source.getWidth(),
			h = source.getHeight(),
			j = w * h,
			i = j - 1,
			x,
			y;

		SampleModel smSRC, smDST;
		WritableRaster wrSRC, wrDST;
		DataBuffer dbSRC, dbDST;
		
		wrSRC = watershed.getRaster();
		smSRC = wrSRC.getSampleModel();
		dbSRC = wrSRC.getDataBuffer();

		int [] lbl;
		int [] src;

		wss = new WSStat(source, nbRegions);

		iSelec = (source.getType() != BufferedImage.TYPE_BYTE_GRAY)?
				(new ColorConvertOp(null).filter(source, new BufferedImage(w, h, BufferedImage.TYPE_BYTE_GRAY))):
				(source);

		wrDST = iSelec.getRaster();
		smDST = wrDST.getSampleModel();
		dbDST = wrDST.getDataBuffer();

		lbl = new int [j];
		src = new int [j];

		for (y = h - 1; y >= 0; y--)
			for (x = w - 1; x >= 0; x--, i--)
				lbl[i] = (smSRC.getSample(x, y, 2, dbSRC) & 0xff) << 16 |
						(smSRC.getSample(x, y, 1, dbSRC) & 0xff) << 8 |
						(smSRC.getSample(x, y, 0, dbSRC) & 0xff);

		smDST.getSamples(0, 0, w, h, 0, src, dbDST);

		for (y = h - 1, i = j - 1; y >= 0; y--)
			for (x = w - 1; x >= 0; x--, i--)
				wss.ajouter(new WSPoint(x, y, src[i]), lbl[i]);
	}

	public String toString () {
		return "Statistique ...";
	}

}