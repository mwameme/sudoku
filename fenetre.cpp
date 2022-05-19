#include "fenetre.h"
#include <QApplication>
#include <QtWidgets>
#include <QDate>
#include <QString>
#include <string>
#include <vector>
#include <QTextEdit>
#include <iostream>
#include <QButtonGroup>
#include <QCheckBox>



using namespace std;

Fenetre::Fenetre()
{

    QVBoxLayout* fenetreLayout = new QVBoxLayout;
    string mastring;
    mastring = "programmé en C++ par Gustave Robichon";
    mastring += '\n';
    mastring += '\n';
    mastring+= "lire readme.txt";
    mastring+= '\n';
    mastring+= '\n';
    mastring+="température : entrer un nombre dans la case en bas";


    setWindowTitle("sudoku");


    QTextEdit* texte = new QTextEdit();
    texte->setPlainText(QString::fromStdString(mastring));
    //QGroupBox* groupeBouton = new QGroupBox;
    QVBoxLayout* groupeBouton = new QVBoxLayout;

    QPushButton *ok=new QPushButton("résoudre");



    m_group = new QButtonGroup;
    m_group->setExclusive(true);
    QCheckBox* checkbox1 = new QCheckBox("niveau un : aléatoire");
    QCheckBox* checkbox2 = new QCheckBox("niveau un : minimum");
    QCheckBox* checkbox4 = new QCheckBox("niveau un : temperature");
    QCheckBox* checkbox3 = new QCheckBox("profondeur : aléatoire");
    m_group->addButton(checkbox1,1);
    m_group->addButton(checkbox2,0);
    m_group->addButton(checkbox3,2);
    m_group->addButton(checkbox4,3);

    checkbox2->setChecked(true);


    groupeBouton->addWidget(checkbox2); //niveau un minimum, 0
    groupeBouton->addWidget(checkbox4); // niveau un temperature, 3
    groupeBouton->addWidget(checkbox1); //niveau un aleatoire, 1
    groupeBouton->addWidget(checkbox3); // profondeur aleatoire, 2

    m_entree_temperature= new QDoubleSpinBox();
    m_entree_temperature->setMaximum(1000);
    m_entree_temperature->setMinimum(0.001);



    fenetreLayout->addWidget(texte);
    fenetreLayout->addLayout(groupeBouton);
    fenetreLayout->addWidget(m_entree_temperature);
    fenetreLayout->addWidget(ok);


    setLayout(fenetreLayout);

    show();

    m_sudoku=NULL;
    m_afficherCalcul=NULL;
    m_afficherSolution=NULL;
    m_afficherHypothese=NULL;
    m_texte= texte;
    m_afficherNbrHypotheses=NULL;
    m_profondeur= NULL;
    m_fenetre_resultat= NULL;

    QWidget::connect(ok,SIGNAL(clicked()),this,SLOT(genererSolution()));

}

Fenetre::~Fenetre()
{
    if(m_sudoku)
        delete m_sudoku;
    if (m_afficherCalcul)
        delete m_afficherCalcul;
    if (m_afficherSolution)
        delete m_afficherSolution;
    if (m_afficherHypothese)
        delete m_afficherHypothese;
    if (m_afficherNbrHypotheses)
        delete m_afficherNbrHypotheses;
    if (m_fenetre_resultat)
        delete m_fenetre_resultat;

}

void Fenetre::genererSolution(){
    string texte_sudoku = m_texte->toPlainText().toStdString();
    if (m_afficherCalcul)
        delete m_afficherCalcul;
    if( m_sudoku)
        delete m_sudoku;
    if( m_profondeur)
        delete m_profondeur;
    if (m_afficherSolution)
        delete m_afficherSolution;
    if (m_afficherHypothese)
        delete m_afficherHypothese;
    if (m_afficherNbrHypotheses)
        delete m_afficherNbrHypotheses;
    if(m_fenetre_resultat)
        delete m_fenetre_resultat;

    m_fenetre_resultat=NULL;
    m_afficherNbrHypotheses=NULL;
    m_afficherCalcul=NULL;
    m_sudoku=NULL;
    m_profondeur=NULL;
    m_afficherSolution=NULL;
    m_afficherHypothese=NULL;


    m_erreur_carte = true;

    string s_iterer, s_generer, s_solution;
    int nbrHypo;
   // cout << m_group->checkedId() <<" choix "<<endl;
    cout << m_group->checkedId() <<endl;


    if (m_group->checkedId() != 2){
        m_sudoku=new Essai(texte_sudoku);
        m_sudoku->m_temperature = m_entree_temperature->value();

//        if (m_sudoku->m_temperature <= 0.01)
//            m_sudoku->m_temperature  = 0.01;
        m_sudoku->m_temperature = m_sudoku->m_temperature;
        m_sudoku->m_temperature = m_sudoku->m_temperature * 0.05;

        m_sudoku->faireCalculer();

//        m_erreur_carte = m_sudoku->m_erreur_carte;

        m_sudoku->m_aleatoire = m_group->checkedId();
        //cout << m_sudoku->m_aleatoire<<" aleatoire "<<endl;

        m_erreur_carte = m_sudoku->iterer();
        s_iterer = m_sudoku->getStringVal();

        m_erreur_carte = m_sudoku->generer();
        vector<vector<int>> tableau(9,vector<int> (9,0));
        /*for(int i(0);i<9;++i)
            for(int j(0);j<9;++j)
                tableau[i][j]=m_sudoku->m_liste_cellules[i][j].m_valeur;*/
        nbrHypo = m_sudoku->getSolution(tableau) -1 ;

        for(int j(0);j<9;++j){
            for(int i(0);i<9;++i){
                s_generer += std::to_string(tableau[i][j]);
                s_generer += " ";
               }
            s_generer+='\n';
        }

        vector<vector<int>> tableau2(9,vector<int> (9,0));
        for(int i(0);i<9;++i)
            for(int j(0);j<9;++j)
                tableau2[i][j]=m_sudoku->m_liste_cellules[i][j].m_valeur;
        m_sudoku->getSolution(tableau2);

        Essai* carteFinale = m_sudoku->derniereCarte();

        //cout << carteFinale << " adresse " <<endl;

        for(int j(0);j<9;++j){
            for(int i(0);i<9;++i){
                s_solution += std::to_string( carteFinale->m_liste_cellules[i][j].m_valeur );
                s_solution += ' ';
            }
            s_solution +='\n';
        }


        delete m_sudoku;
        m_sudoku = NULL;
    }
    else{
        m_sudoku = NULL;
        m_profondeur =new Profondeur(texte_sudoku);
        m_profondeur->faireCalculer();

        m_erreur_carte = m_profondeur->m_erreur_carte;

        s_iterer=m_profondeur->getStringVal();

//        m_profondeur->genererPossible();

        if (m_erreur_carte)
            m_erreur_carte = m_profondeur->boucle();
        else
            m_profondeur->boucle();

        vector<vector<int>> hypotheses(9,vector<int> (9,0));
        nbrHypo = m_profondeur->recuperer(hypotheses);
        //cout << nbrHypo <<" nbr Hypo " << endl;
        Carte* derniereCarte= m_profondeur->derniereCarte();


        for(int j(0);j<9;++j){
            for(int i(0);i<9;++i){
                s_generer += std::to_string(hypotheses[i][j]);
                s_generer += " ";
               }
            s_generer+='\n';
        }

        s_solution= derniereCarte->getStringVal();

        delete m_profondeur;
        m_profondeur=NULL;


    }

    s_iterer = '\n' + s_iterer;
    s_iterer = '\n' + s_iterer;
    s_iterer = "premier calcul :" + s_iterer;
    m_afficherCalcul = new QTextEdit;
    m_afficherCalcul->setPlainText(QString::fromStdString(s_iterer));
    //m_afficherCalcul->setObjectName("premier calcul");

    s_generer = '\n' + s_generer;
    s_generer = '\n' + s_generer;
    s_generer = "hypotheses ajoutées :" + s_generer;
    m_afficherHypothese = new QTextEdit;
    m_afficherHypothese->setPlainText(QString::fromStdString(s_generer));
    //m_afficherHypothese->setWindowTitle("hypotheses ajoutées");

    s_solution ='\n' + s_solution;
    s_solution ='\n' + s_solution;
    s_solution = "solution avec hypotheses : " + s_solution;
    m_afficherSolution= new QTextEdit;
    m_afficherSolution->setPlainText(QString::fromStdString(s_solution));
    //m_afficherSolution->setWindowTitle("solution avec hypotheses");

    string nbrHypoString=to_string(nbrHypo);
    nbrHypoString = "nombre d'hypotheses supplémentaires : " + nbrHypoString;
    nbrHypoString = nbrHypoString + '\n';
    if (m_erreur_carte == true)
        nbrHypoString = nbrHypoString + "pas d'erreur dans la donnée de la carte";
    else
        nbrHypoString = nbrHypoString + "erreur dans la donnée de la carte";

    m_afficherNbrHypotheses=new QTextEdit;
    m_afficherNbrHypotheses->setPlainText(QString::fromStdString(nbrHypoString));
    //m_nbrHypotheses->setWindowTitle("nombre Hypotheses :");

    QGridLayout * fenetre2_layout= new QGridLayout;
    fenetre2_layout->addWidget(m_afficherCalcul,0,0,1,1);
    fenetre2_layout->addWidget(m_afficherHypothese,0,1,1,1);
    fenetre2_layout->addWidget(m_afficherSolution,1,0,1,1);
    fenetre2_layout->addWidget(m_afficherNbrHypotheses,1,1,1,1);

    m_fenetre_resultat = new QWidget;
    m_fenetre_resultat->setLayout(fenetre2_layout);
    m_fenetre_resultat->setWindowTitle("résolution");
    m_fenetre_resultat->show();


}
