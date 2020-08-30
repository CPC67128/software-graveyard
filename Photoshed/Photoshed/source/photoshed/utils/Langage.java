package photoshed.utils;

import java.lang.reflect.Field;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.FileInputStream;
import java.io.BufferedReader;
import java.io.InputStreamReader;

import photoshed.Photoshed;
import photoshed.fenetres.Erreur;

public final class Langage {

	/*****************************************************
	*	Chaine de caract�re global				*
	*****************************************************/

	public String G_LOAD = "Chargement : ";
	
	public String G_READY = "Pr�t";

	public String G_LANGAGE_FILENAME = "Francais";

	public String G_CFG = "Propri�t�s";

	public String G_LANGAGE = "Langage";

	public String G_MENUBAR = "Barre de menu";

	public String G_INTERFACE = "Interface du logiciel";

	public String G_TOOLBOX = "Bo�te � outils";

	/*****************************************************
	*	Chaine de caract�re pour les menus			*
	*****************************************************/

	// Menu Fichier

	public String M_FILE = "Fichier";

	//public String M_FILE_NEW;
	
	public String M_FILE_OPEN = "Ouvrir ...";

	public String M_FILE_SAVE = "Sauvegarder";

	public String M_FILE_SAVEAS = "Sauvegarder sous ...";
	
	public String M_FILE_CLOSE = "Fermer";

	public String M_FILE_OPTION = "Pr�f�rences ...";

	public String M_FILE_RECENT = "Fichiers r�cents";
	
	public String M_FILE_QUIT = "Quitter";

	// Menu des filtres

	public String M_FILTERS = "Filtres";

	// Menu Aide
	
	public String M_HELP = "Aide";
	
	public String M_HELP_CONTENTS = "Rubrique d'aide";
	
	public String M_HELP_ABOUT = "A propos de ...";
	
	// Menu Fen�tre

	public String M_WINDOW = "Fen�tres";

	public String M_WINDOW_TILE = "R�organiser";

	public String M_WINDOW_CASCADE = "Cascade";

	public String M_WINDOW_CLOSEALL = "Tout fermer";

	/*****************************************************
	*	Chaine de caract�re des messages d'erreurs	*
	*****************************************************/

	public String E_READ = "Erreur de lecture dans le fichier : ";

	public String E_WRITE = "Erreur d'�criture dans le fichier : ";

	public String E_STOP = "Erreur fatale : le programme va s'arr�ter";

	public String E_INVAL = "Argument invalide : ";

	public String E_NOMEM = "M�moire insuffisante";

	public String E_NOFILE = "Fichier inexistant : ";

	public String E_LOAD_CLASS = "Une erreur s'est produite lors du chargement de la class : ";

	public String E_LOAD_LANGUAGE = "Impossible de charger correctement la langue selectionn�e : ";

	public String E_LOAD_PROPERTIES = "Le chargement complet des propri�t�s a �chou� !";

	public String E_LOAD_MENU = "Chargement des menus impossible";

	public String E_DFLTCFG = "Utilisation du fichier de configuration par defaut";

	public String E_LOADER = "R�cup�ration du chargeur de class impossible";

	public String E_NOFLD = "Nom de variable incorrect : ";

	public String E_FLDACC = "Changement de valeur de la variable impossible : ";

	public String E_CHGWIN = "Le changement de fen�tre a �chou�";

	public String E_NOTEXCEPTED = "Erreur inattendue";

	/*****************************************************
	*	Chaine de caract�re des boites de dialogues	*
	*****************************************************/

	public String DB_OPEN = "Selectionner un fichier � ouvrir ";

	public String DB_SAVEAS = "Sauvegarder le fichier sous ...";

	public String DB_OPTION = "Pr�f�rences :";

	public String DB_OK = "Ok";

	public String DB_CANCEL = "Annuler";

	public String DB_APPLY = "Appliquer";

	public String DB_RECENT = "Nombre de fichiers r�cents � m�moriser : ";

	public String DB_LNGSELECT = "Selectionnez une langue :";

	public String DB_SAVE = "Enregistrer";

	public String DB_COLOR = "Ouvrir en couleur";

	public String DB_SAVEWIN = "Sauvegarder la position et la taille de la fen�tre principale";

	public String DB_CLEARLAST = "Effacer les fichiers r�cents";

	public String DB_PIC = "Cr�er une nouvelle image apr�s chaque filtre";

	public String DB_SHOWSPLASH = "Afficher l'image d'intro";

	/*****************************************************
	*	Le type "Langage"						*
	*****************************************************/

	public Langage() {
	}

	public Langage(String langage) {
		setLangage(langage);
	}

	public void update() {
		langLoad(G_LANGAGE_FILENAME);
	}

	public void setLangage(String l) {
		if (!(G_LANGAGE_FILENAME.equals(l))) {
			langLoad(l);
		}
	}

	protected void langLoad(String l) {
		String texte,
			fieldsName = "",
			fieldsValue,
			nomClass = "photoshed.utils.Langage";

		boolean erreur = false;

		int equalsIndex = 0;

		try {
			ClassLoader classL = ClassLoader.getSystemClassLoader();
			Class cls = classL.loadClass(nomClass);

			BufferedReader lr = new BufferedReader(
							new InputStreamReader(
								new FileInputStream(
									new File(l + ".wlg") ) ) );
			G_LANGAGE_FILENAME = l;

			for (int i = 0; i < cls.getFields().length - 1; i++) {
				texte = lr.readLine();
				equalsIndex = texte.indexOf("=");
				fieldsName = texte.substring(0, equalsIndex);
				fieldsValue = texte.substring(equalsIndex + 1);
				try {
					cls.getField(fieldsName).set(this, fieldsValue);
				}
				catch(NoSuchFieldException e) {
					Erreur.print((Exception)e, E_NOFLD + fieldsName);
					erreur = true;
				}
				catch(IllegalAccessException e) {
					Erreur.print((Exception)e, E_FLDACC + fieldsName);
					erreur = true;
				}
			}
			/*try {
				cls.getField("G_LANGAGE_FILENAME").set(this, l);
			}
			catch(NoSuchFieldException e) {
				Erreur.print((Exception)e, E_NOFLD + fieldsName);
				erreur = true;
			}
			catch(IllegalAccessException e) {
				Erreur.print((Exception)e, E_FLDACC + fieldsName);
				erreur = true;
			}*/
			if (erreur)
				throw new Exception();
		}
		catch(ClassNotFoundException e) {
			Erreur.print((Exception)e, E_LOAD_CLASS + nomClass);
		}
		catch(FileNotFoundException e) {
			Erreur.print((Exception)e, E_NOFILE + l + ".wlg");
		}
		catch(IOException e) {
			Erreur.print((Exception)e, E_READ + l + ".wlg");
		}
		catch(SecurityException e) {
			Erreur.print((Exception)e, E_LOADER);
		}
		catch(Exception e) {
			Erreur.print(e, E_LOAD_LANGUAGE + l);
		}
	}
}