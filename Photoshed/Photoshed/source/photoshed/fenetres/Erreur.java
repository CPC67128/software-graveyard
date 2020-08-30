package photoshed.fenetres;

import javax.swing.JDialog;
import javax.swing.JButton;
import javax.swing.JTextArea;
import javax.swing.SwingUtilities;
import javax.swing.WindowConstants;
import javax.swing.JComponent;

import java.awt.Frame;
import java.awt.Container;
import java.awt.Component;
import java.awt.BorderLayout;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

public final class Erreur extends JComponent implements ActionListener {

	private JButton ok;

	private JTextArea text;

	JDialog dialog = null;

	public Erreur() {
		ok = new JButton();
		ok.addActionListener(this);

		text = new JTextArea(8, 30);
		text.setLineWrap(true);
		text.setWrapStyleWord(true);
		text.setEditable(false);
		text.setVisible(true);

		setLayout(new BorderLayout());
		add(text, BorderLayout.CENTER);
		add(ok, BorderLayout.SOUTH);
	}
	
	public static void print(Throwable e, String msg) {
		System.err.println(msg);
		e.printStackTrace();
	}

	public void show(Component parent, Throwable e, String title, String msg, String buttonText) {
		if (e != null) e.printStackTrace();

		Frame frame = parent instanceof Frame ? (Frame) parent
              : (Frame)SwingUtilities.getAncestorOfClass(Frame.class, parent);

		dialog = new JDialog(frame, title, true);
		dialog.setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);

		ok.setText(buttonText);
		text.setText(msg);

		dialog.setContentPane(this);
		dialog.pack();
        dialog.setLocationRelativeTo(parent);
		dialog.show();
	}

	public void actionPerformed(ActionEvent ae) {
		dialog.hide();
		dialog.dispose();
		dialog = null;
	}
}