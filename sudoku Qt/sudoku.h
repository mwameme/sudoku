#ifndef SUDOKU_H
#define SUDOKU_H

#include <string>
#include <vector>

#include <ctime> // Obligatoire
#include <cstdlib> // Obligatoire
#include <math.h>       /* exp */



//on fait setValue depuis la carte :
//on appelle cellule.setValue
//puis on appelle  ligne/colonne/carré.setValue
//quand on a 1 case de trouvée, on appelle setValue depuis la carte !


// on met des bool partout, pour que si cellule.setOff return false, on s'arrête !
//donc les autres fonctions aussi alors retournent false,
//et on livre le message d'erreur !!!

class Carte;

class Cellule{
public :
    int m_x;
    int m_y;
    Carte* m_carte;

    std::vector<bool> m_listeNum; //les 9 possibles
    int m_valeur; //si fixe
    int m_taille; //nombre des possibles

    bool setOff(int num);
    bool setValeur(int num);

    Cellule();
    Cellule(int num);
    Cellule(Cellule const& cellule, Carte* carte);

    Cellule(Cellule const& cellule);

};

class Ligne{
public:
    int m_y;
    Carte* m_carte;

    std::vector<Cellule*> m_cellules; //la liste des cellules : 9
    std::vector<std::vector<Cellule*>> m_liste_num; //liste, pour chaque numero,
                                            //des cellules qui ont ce possible :
                                            //m_liste_num[num][cellule_i]

    Ligne();
    Ligne(int j,Carte* carte); //prend la ligne (i) depuis la carte;
    Ligne(Ligne const& ligne, Carte* carte);

    bool setOff(int x,int val); //change m_liste_
    bool setValeur(int x,int val);//change m_liste_

};

class Colonne{
public :
    int m_x;
    Carte* m_carte;

    std::vector<Cellule*> m_cellules; //la liste des cellules
    std::vector<std::vector<Cellule*>> m_liste_num; //liste, pour chaque numero,
                                            //des cellules qui ont ce possible :
                                            //m_liste_num[num][cellule_i]
    Colonne();
    Colonne(int i,Carte* carte); //prend la colonne (j) depuis la carte;
    Colonne(Colonne const& colonne, Carte* carte);

    bool setOff(int y,int val); //change m_liste_
    bool setValeur(int y,int val); //change m_liste_
};

class Carre{
public :
    int m_c;
    Carte* m_carte;

    std::vector<Cellule*> m_cellules; //la liste des cellules
    std::vector<std::vector<Cellule*>> m_liste_num; //liste, pour chaque numero,
                                            //des cellules qui ont ce possible :
                                            //m_liste_num[num][cellule_i]
    Carre();
    Carre(int c,Carte* carte); //prend le carre (i,j) depuis la carte;
    Carre(Carre const& carre, Carte* carte);

    bool setOff(int k,int val); //change m_liste_
    bool setValeur(int k,int val); //change m_liste_
};

class Carte{
public :
    bool m_modifie;
    int m_modification;
    bool m_erreur_carte;
    std::vector<std::vector<Cellule>> m_liste_cellules; //les cellules

    std::vector<Ligne> m_liste_lignes; //les lignes
    std::vector<Colonne> m_liste_colonnes; //les colonnes
    std::vector<Carre> m_liste_carres; //les carrés

    bool setOff(int x,int y,int val); //modifie toute la carte ...
    bool setValeur(int x,int y,int val);


    Carte();
    Carte(Carte const& carte); //constructeur de copie : recopie les cellules,
                            //et met à jour les pointeurs !!!
    Carte(std::string string_sudoku); //ouvre un fichier, et met la Carte à jour ...
    //~Carte(); //comme les cellules sont détruites, il n'y a rien à y faire ...

    //void forcerCalcul(); //fait les non-
    bool calculerSi();
    bool calculerDoublet();
    bool calculerTriplet();
    bool faireCalculer();


    std::string getStringVal() const;
//    std::string getStringTaille() const;
};

class Test : public Carte{
public:
    int m_x;
    int m_y;
    int m_val;
    bool m_erreur; //false si c'est une erreur !
    //une carte, qui suppose m_val en m_x * m_y. Possible ou non ...
    Test();
    Test(Carte const& carte);
    Test(std::string string_sudoku);
    //bool tester(int x,int y,int val);
};

class Essai : public Test {//c'est une carte (en héritage) avec une liste de possibles ... récurrence sur les essais faux !
public :
    std::vector<Essai*> m_liste_tests; //les différents essais;
        //this->carte : on modifie si test.possible == false
        //et on modifie donc dans tous les tests ...

        //si un Test est faux, on l'enleve de la liste, et on MET FAUX pour tous les tests !
        //si un essai est faux, modifie la carte ...
    Essai* m_solution;
    Essai* m_mere;

    bool ajouterTest(int x,int y,int val);
    bool enlever(int x,int y,int val);
    bool iterer();
    bool tester(int x,int y,int val);
    void nettoyerListe();

//    bool niveauDeux();
    bool generer();

    int nombrePossible();
    int getSolution(std::vector<std::vector<int>>& tableau);
    Essai* derniereCarte();
    void viderListe();
    void ordreSemiAleatoire();


    //part d'une carte, et liste les essais à tester, et itere les calculs !
    //Essai(Carte& carte);
    Essai(std::string string_sudoku);
//    Essai(Carte const& carte);
    Essai(Essai const& essai);
    Essai();
    ~Essai();

    int m_aleatoire;
    double m_temperature;
    double m_fact_alea;

};

void melangerListe(std::vector<Essai*>& mon_vecteur);


std::ostream& operator<<( std::ostream &flux, Essai const& essai );
std::ostream& operator<<( std::ostream &flux, Test const& test );
std::ostream& operator<<( std::ostream &flux, Carte const& carte );

void resoudreGenererTester(std::string adresse);

class comparaison_modification
{
public :
    bool operator()(const Essai* struct1, const Essai* struct2);

};

class Profondeur : public Carte{
public :
    Profondeur* m_niveauSup;
    Cellule m_possible;


    bool choisirPossible();
    bool essayer();
    bool boucle();

    int recuperer(std::vector<std::vector<int>>& tableau);

    Profondeur(Profondeur& parent);
    Profondeur(Carte& carte);

    Profondeur(std::string string_sudoku);
    ~Profondeur();
    Profondeur* derniereCarte();
};



#endif // SUDOKU_H_INCLUDED
