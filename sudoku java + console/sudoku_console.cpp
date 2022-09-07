// sudoku_console.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <string>
#include <vector>
#include <ctime>  //initialiser rand
#include <fstream> //fichiers
#include <cstdlib> // rand
#include "sudoku.h"

using namespace std;


int main(int argc, char* argv[])
{
    srand(static_cast<unsigned>(time(0)));
	bool continuer;

    fstream fichier;
    fichier.open("sudoku.txt", ios_base::in);
    if (!fichier.is_open())
        throw("echec ouverture fichier");
    char choix_char;
    char chaine_entree[200];
    if (!fichier.eof())
        fichier.getline(chaine_entree,199,'\n');
    choix_char = chaine_entree[0];
//	cout << "ouerture fichier" << endl;
//	cin >> continuer;

	int choix = -1;

	if (choix_char == '0')
		choix = 0;
	else if (choix_char == '1')
		choix = 1;
	else if(choix_char == '2')
		choix = 2;
	else if(choix_char == '3')
		choix = 3;
	else if (choix_char == '4')
		choix = 4;

	if (choix == -1)
		throw ("choix incorrect");
	double temperature = 0;
	if (choix == 3) {
		if ((chaine_entree[1] == '\0'))
			throw("temperature non specificée");
		string mon_reel(chaine_entree + 2, 10);
		temperature = std::stod(mon_reel);
		if (temperature <= 0)
			throw("temperature negative ou nulle");
//		cout << "temperature : " << temperature << endl;
	}
	else temperature = 1;

    fichier.get(chaine_entree, 199, EOF);
    fichier.close();
    if (fichier.is_open())
        throw ("echec fermeture fichier");


    string string_sudoku(chaine_entree);
//	cout << "choix : " << choix_char << endl;
//	cout << "get sudoku" << endl << string_sudoku << endl;
//	cin >> continuer;



	string s_iterer, s_generer, s_solution;
	int nbrHypo=0;
	// cout << m_group->checkedId() <<" choix "<<endl;
	bool m_erreur_carte = true;


	//niveau un minimum, 0
	// niveau un temperature, 3
	//niveau un aleatoire, 1
	//profondeur aleatoire, 2
	//juste premier calcul : 4
	Carte* carte = NULL;
	Essai* m_sudoku = NULL;
	if (choix == 4) { //juste calculer;
		carte = new Carte(string_sudoku);
		s_iterer = carte->getStringVal();
		delete carte;
	}
	else if (choix != 2) {
		m_sudoku = new Essai(string_sudoku);
		m_sudoku->m_temperature = temperature;

		if (m_sudoku->m_temperature <= 0.001)
			m_sudoku->m_temperature = 0.001;

		m_sudoku->m_temperature = m_sudoku->m_temperature * 0.01;
		//        m_erreur_carte = m_sudoku->m_erreur_carte;
		m_sudoku->m_aleatoire = choix;
		//cout << m_sudoku->m_aleatoire<<" aleatoire "<<endl;

		m_erreur_carte = m_sudoku->m_erreur_carte;
		if (m_erreur_carte) {
			m_erreur_carte = m_sudoku->iterer();
			s_iterer = m_sudoku->getStringVal();
		}
//		else
//			s_iterer = "";
		
//		cout << "fin premiere iteration " << endl << endl;
//		cout << s_iterer << endl;
//		cin >> continuer;

		if (m_erreur_carte)
			m_erreur_carte = m_sudoku->generer();

//		cout << "fin generer " << endl;
//		cin >> continuer;

		if (m_erreur_carte) {
			vector<vector<int>> tableau(9, vector<int>(9, 0));
			/*for(int i(0);i<9;++i)
				for(int j(0);j<9;++j)
					tableau[i][j]=m_sudoku->m_liste_cellules[i][j].m_valeur;*/
			nbrHypo = m_sudoku->getSolution(tableau) - 1;

			for (int j(0); j < 9; ++j) {
				for (int i(0); i < 9; ++i) {
					s_generer += std::to_string(tableau[i][j]);
					s_generer += " ";
				}
				s_generer += '\n';
			}

			vector<vector<int>> tableau2(9, vector<int>(9, 0));
			for (int i(0); i < 9; ++i)
				for (int j(0); j < 9; ++j)
					tableau2[i][j] = m_sudoku->m_liste_cellules[i][j].m_valeur;
			m_sudoku->getSolution(tableau2);

			Essai* carteFinale = m_sudoku->derniereCarte();

			//cout << carteFinale << " adresse " <<endl;

/*			for (int j(0); j < 9; ++j) {
				for (int i(0); i < 9; ++i) {
					s_solution += std::to_string(carteFinale->m_liste_cellules[i][j].m_valeur);
					s_solution += ' ';
				}
				s_solution += '\n';
			}*/
			s_solution = carteFinale->getStringVal();
		}
/*		else {
			s_solution = "";
			s_generer = "";
		}*/

		delete m_sudoku;
		m_sudoku = NULL;
	}
	else { //choix == 2 : profondeur
		m_sudoku = NULL;
		Profondeur* m_profondeur = new Profondeur(string_sudoku);

		m_erreur_carte = m_profondeur->m_erreur_carte;

		s_iterer = m_profondeur->getStringVal();

		//        m_profondeur->genererPossible();

		if (m_erreur_carte)
			m_erreur_carte = m_profondeur->boucle();

		if (m_erreur_carte) {

			vector<vector<int>> hypotheses(9, vector<int>(9, 0));
			nbrHypo = m_profondeur->recuperer(hypotheses);
			//cout << nbrHypo <<" nbr Hypo " << endl;
			Profondeur* derniereCarte = m_profondeur->derniereCarte();


			for (int j(0); j < 9; ++j) {
				for (int i(0); i < 9; ++i) {
					s_generer += std::to_string(hypotheses[i][j]);
					s_generer += " ";
				}
				s_generer += '\n';
			}

			s_solution = derniereCarte->getStringVal();
		}
/*		else {
			s_generer = "";
			s_solution = "";
		}*/

//		cout << "derniere modification : " << m_profondeur->deniere_modification() << endl;
//		cin >> continuer;


		delete m_profondeur;
		m_profondeur = NULL;


	}

//	if (!m_erreur_carte)
//		throw("erreur carte");

	//premier calcul
	s_iterer = '\n' + s_iterer;
	s_iterer = '\n' + s_iterer;
	s_iterer = "premier calcul :" + s_iterer; 

	//hypothèses
	s_generer = '\n' + s_generer;
	s_generer = '\n' + s_generer;
	s_generer = "hypotheses ajoutées :" + s_generer;

	//solution finale
	s_solution = '\n' + s_solution;
	s_solution = '\n' + s_solution;
	s_solution = "solution avec hypotheses : " + s_solution;

	string s_hypo = to_string(nbrHypo);
	s_hypo = "nombre d'hypotheses supplémentaires : " + s_hypo;
	s_hypo = s_hypo + '\n';
	if (m_erreur_carte == true)
		s_hypo = s_hypo + "pas d'erreur dans la donnée de la carte";
	else
		s_hypo = s_hypo + "erreur dans la donnée de la carte";
	s_hypo = s_hypo + '\n';
	switch (choix) {
	case 0: s_hypo = s_hypo + "niveau un : minimum";
		break;
	case 1: s_hypo = s_hypo + "niveau un : aleatoire";
		break;
	case 2: s_hypo = s_hypo + "profondeur : aleatoire";
		break;
	case 3: s_hypo = s_hypo + "niveau un : semi-aleatoire";
		break;
	case 4: s_hypo = s_hypo + "calcul simple";
		break;
	}


/*	char char_hypo[s_hypo.length() + 1];
	char char_solution[s_solution.length() + 1];
	char char_generer[s_generer.length() + 1];
	char char_iterer[s_iterer.length() + 1];*/


	const char* char_hypo=s_hypo.c_str();
	const char* char_solution = s_solution.c_str();
	const char* char_generer= s_generer.c_str();
	const char* char_iterer =s_iterer.c_str();

	fichier.open("premier calcul.txt", ios_base::out);
	if (fichier.is_open()) {
		fichier << char_iterer;
		fichier.close();
		if (fichier.is_open())
			throw("erreur fermeture fichier premier calcul");
	}
	else
		throw ("erreur ouverture fichier premier calcul");

//	cout << "fin premier fichier !" << endl;
//	cin >> continuer;



	fichier.open("hypotheses.txt", ios_base::out);
	if (fichier.is_open()) {
		fichier << char_generer;
		fichier.close();
		if (fichier.is_open())
			throw("erreur fermeture fichier hypotheses");
	}
	else
		throw ("erreur ouverture fichier hypotheses");

	fichier.open("solution.txt", ios_base::out);
	if (fichier.is_open()) {
		fichier << char_solution;
		fichier.close();
		if (fichier.is_open())
			throw("erreur fermeture fichier solution");
	}
	else
		throw ("erreur ouverture fichier solution");

	fichier.open("resume.txt", ios_base::out);
	if (fichier.is_open()) {
		fichier << char_hypo;
		fichier.close();
		if (fichier.is_open())
			throw("erreur fermeture fichier erreur");
	}
	else
		throw ("erreur ouverture fichier erreur");

	return 1;

}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
