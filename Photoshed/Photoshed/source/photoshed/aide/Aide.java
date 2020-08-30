/****************************************************
 *                                                  *
 *  Projet T5 "Traitement d'images"                 *
 *                                                  *
 *    Auteurs : FUCHS Steve                         *
 *              HAHN Philippe                       *
 *              SCHEFFKNECHT Fran�ois               *
 *                                                  *
 ****************************************************
 *                                                  *
 *  Fichier Aide.java                               *
 *                                                  *
 *  Ce fichier contient les classe utilis�es dans   *
 *  le cadre du manuel d'utilisation accessible     *
 *  depuis l'interface.                             *
 *                                                  *
 ****************************************************/


// D�signation du package d'appartenance
package photoshed.aide;


// Importations des librairies JAVA n�c�ssaires
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Toolkit;
import java.awt.event.WindowListener;
import java.awt.event.WindowEvent;
import javax.swing.event.HyperlinkListener;
import javax.swing.JTextPane;
import javax.swing.JEditorPane;
import javax.swing.text.html.HTMLEditorKit;
import javax.swing.JFrame;
import java.io.IOException;
import java.io.File;
import java.net.URL;
import java.net.MalformedURLException;
import javax.swing.event.HyperlinkListener;
import javax.swing.event.HyperlinkEvent;
import javax.swing.text.html.HTMLFrameHyperlinkEvent;
import javax.swing.text.html.HTMLDocument;
import photoshed.Photoshed;


// D�but de la classe Aide
public class Aide extends JFrame implements WindowListener, HyperlinkListener
{
	// ***** D�clarations des attributs priv�s
	private Photoshed photoshed;


	// ***** Constructeur des objets de la classe Aide
	public Aide(Photoshed photoshed)
	{
		// Appel du constructeur de la classe m�re
		super("Manuel en ligne de l'application");

		// Mise � niveau des variables locales
		this.photoshed = photoshed;

		// D�finitions des options de pr�sentation
		Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
		this.setSize(screen.width, screen.height);

		this.getContentPane().setLayout(new BorderLayout());
		this.setLocation(0, 0);

		// Affichage d'une zone HTML
		JEditorPane explorer = new JEditorPane();
		explorer.setEditable(false);

		HTMLEditorKit kit = new HTMLEditorKit();
		kit.install(explorer);
		explorer.addHyperlinkListener(this);

		this.getContentPane().add(explorer, BorderLayout.CENTER);

		try
		{
			File fichier = new File(new String("aide" + File.separatorChar + "html" + File.separatorChar + "Aide.htm"));
			URL pageHTML = fichier.toURL();
			explorer.setPage(pageHTML);
		}
		catch (MalformedURLException e)
		{
			System.out.println("Un probl�me est survenue lors de la converion du fichier en URL...");
		}
		catch (IOException e)
		{
			System.out.println("Le chemin d'acc�s au fichier HTML est �rron� !");
		}

		// Affectation d'un WindowListener � la fen�tre de dialogue
		addWindowListener(this);

		// Affichage de la fen�tre de dialogue
		this.setVisible(true);
		this.toFront();
	}

	// Fonctions relatives au fonctionnement du WindowListener
	public void windowActivated(WindowEvent we)		{}
	public void windowClosed(WindowEvent we)		{}
	public void windowDeactivated(WindowEvent we)	{}
	public void windowDeiconified(WindowEvent we)	{}
	public void windowOpened(WindowEvent we)		{}
	public void windowIconified(WindowEvent we)		{}
	public void windowClosing(WindowEvent we) {
		photoshed.closeHelp();
	}

	// Fonction relative au fonctionnement du HyperlinkListener
	public void hyperlinkUpdate(HyperlinkEvent e)
	{
		if (e.getEventType() == HyperlinkEvent.EventType.ACTIVATED)
		{
			// R�cup�ration de l'�diteur solicit�
			JEditorPane pane = (JEditorPane) e.getSource();

			if (e instanceof HTMLFrameHyperlinkEvent)
			{
				HTMLFrameHyperlinkEvent evt = (HTMLFrameHyperlinkEvent) e;
				HTMLDocument doc = (HTMLDocument) pane.getDocument();
				doc.processHTMLFrameHyperlinkEvent(evt);
			}
			else
			{
				try
				{
					pane.setPage(e.getURL());
				}
				catch (Throwable t)
				{
					t.printStackTrace();
				}
			}
		}
	}
}