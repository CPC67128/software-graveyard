/*---------------------------------------------------*
 |      FDBox.java                                   |
 |---------------------------------------------------|
 | C'est la classe d�finissant un objet JDialof      |
 | permettant � l'utilisateur d'entrer ses options   |
 | de sa fusion.                                     |
 *---------------------------------------------------*/

// D�signation du package d'appartenance
package photoshed.filtres.fusion;

// Importations des librairies JAVA n�c�ssaires
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JFrame;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JSlider;
import javax.swing.JPanel;
import javax.swing.ImageIcon;
import javax.swing.SwingConstants;

import javax.swing.event.ChangeListener;
import javax.swing.event.ChangeEvent;

import java.awt.GridLayout;
import java.awt.Toolkit;
import java.awt.Image;
import java.awt.BorderLayout;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

// D�but de la classe FDBox
public class FDBox extends JDialog implements ChangeListener, ActionListener
{
	// ----- D�claration des attributs de FDBox
	private JLabel	lblSeuil,		// Le label repr�sentant la valeur du seuil 
					lblArea,		// Le label affichant la valeur de l'aire minimale
					lblMinima;		// Le label affichant le nb de labels de moyennes minimales � traiter
	private JCheckBox testArea;		// La case � cocher permettant de choisir l'arr�t de fusion avec l'aire de la zone
	private JSlider	area,			// Le slider permettant de choisir la valeur de l'aire maximale
					seuil,			// Le slider permettant de choisir le seuil du contraste
					minima;			// Le slider des minimas
	private JButton btnOK,			// Le bouton permettant de valider les changements
					btnAnnuler;		// Le bouton permettant d'annuler la fusion
	private boolean estOK = true;	// Le bool�en indiquant si le bouton annuler a �t� actionn�

	// ----- Constructeur
	public FDBox(int height, int width, int nbDifferentLabel, JFrame owner)
	{
		// Appel au constructeur m�re de JDialog
		super(owner, "Param�tres de la fusion", true);

		// Cr�ation de la fen�tre
		this.getContentPane().setLayout(new BorderLayout());	// Affectation d'un LayoutManager
		this.setSize(400, 350);									// Taille de la fen�tre
		this.setLocation(100, 100);								// Localisation initiale de la fen�tre

		// Ajout des composants � cette fen�tre
		JPanel superPanneau = new JPanel(new GridLayout(3, 1));

		// Ajout de l'ensemble "choix du seuil"
		JPanel panneau = new JPanel();
		panneau.setLayout(new BorderLayout(5, 5));
		panneau.add(new JLabel("Valeur du seuil :", JLabel.CENTER), BorderLayout.NORTH);
		panneau.add(seuil = new JSlider(SwingConstants.HORIZONTAL, 0, 255, 20), BorderLayout.CENTER);
		panneau.add(lblSeuil = new JLabel(String.valueOf(20)), BorderLayout.EAST);
		lblSeuil.setVerticalAlignment(SwingConstants.TOP);
		seuil.setMinorTickSpacing(10); seuil.setMajorTickSpacing(50); seuil.createStandardLabels(30); seuil.setPaintLabels(true); seuil.setPaintTicks(true);
		seuil.addChangeListener(this);
		superPanneau.add(panneau);

		// Ajout de l'ensemble "Graduation des labels"
		panneau = new JPanel();
		panneau.setLayout(new BorderLayout(5, 5));
		panneau.add(new JLabel("Graduation des moyennes de couleurs :", JLabel.CENTER), BorderLayout.NORTH);
		panneau.add(minima = new JSlider(SwingConstants.HORIZONTAL, 1, nbDifferentLabel, (int) (nbDifferentLabel / 2)), BorderLayout.CENTER);
		panneau.add(lblMinima = new JLabel(String.valueOf((int) (nbDifferentLabel / 2))), BorderLayout.EAST);
		lblMinima.setVerticalAlignment(SwingConstants.TOP);
		minima.setMinorTickSpacing((int) (nbDifferentLabel / 25 + 1)); minima.setMajorTickSpacing((int) (nbDifferentLabel / 5 + 1)); minima.createStandardLabels((int) (nbDifferentLabel / 25 + 1)); minima.setPaintLabels(true); minima.setPaintTicks(true);
		minima.addChangeListener(this);
		superPanneau.add(panneau);

		// Ajout de l'ensemble "Taille minimale"
		panneau = new JPanel();
		panneau.setLayout(new BorderLayout(5, 5));
		panneau.add(testArea = new JCheckBox("Sp�cifier la taille minimale des r�gions finales", false), BorderLayout.NORTH);
		panneau.add(area = new JSlider(SwingConstants.HORIZONTAL, 0, (height * width) / 2, (height * width) / 4), BorderLayout.CENTER);
		panneau.add(lblArea = new JLabel(String.valueOf((height * width) / 2)), BorderLayout.EAST);
		lblArea.setVerticalAlignment(SwingConstants.TOP);
		testArea.addChangeListener(this);
		area.setEnabled(false);
		area.setMinorTickSpacing((height * width) / 25); area.setMajorTickSpacing((height * width) / 5); area.createStandardLabels((height * width) / 5); area.setPaintLabels(true); area.setPaintTicks(true);
		area.addChangeListener(this);
		superPanneau.add(panneau);

		this.getContentPane().add(superPanneau, BorderLayout.CENTER);

		// Ajout des deux boutons permettant la validation
		panneau = new JPanel(new GridLayout(1, 2));
		panneau.add(btnOK = new JButton("OK"));
		btnOK.addActionListener(this);
		btnOK.setMnemonic('o');
		panneau.add(btnAnnuler = new JButton("Annuler"));
		btnAnnuler.addActionListener(this);
		btnOK.setMnemonic('a');
		this.getContentPane().add(panneau, BorderLayout.SOUTH);

		// Affichage de la fen�tre
		this.show();
	}

	// ----- M�thodes d'instances

	// Proc�dure permettant de g�rer les changements d'�tats des JSliders et de la JCheckBox
	public void stateChanged(ChangeEvent ce)
	{
		if (ce.getSource() == seuil)
			lblSeuil.setText(String.valueOf(seuil.getValue()));
		else if (ce.getSource() == area)
			lblArea.setText(String.valueOf(area.getValue()));
		else if (ce.getSource() == minima)
			lblMinima.setText(String.valueOf(minima.getValue()));
		else if (ce.getSource() == testArea)
			area.setEnabled(testArea.isSelected());
	}

	// Proc�dure permettant de g�r�r les boutons Annuler et OK
	public void actionPerformed(ActionEvent ae)
	{
		if (ae.getSource() == btnAnnuler)
			estOK = false;
		this.setVisible(false);
	}

	// Fonctions permettant de r�cup�rer les valeurs entr�es par l'utilisateur
	public int getSeuil()		{	return (estOK ? seuil.getValue() : (-1));	}
	public int getNbMinima()	{	return minima.getValue();	}
	public int getArea()		{	return ((testArea.isSelected()) ? area.getValue() : (-1));	}
}