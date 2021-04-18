#include "fenetre.h"
#include <QApplication>
#include <QtWidgets>
#include <QDate>
#include <QString>
#include <string>
#include <vector>
#include <QTextEdit>


using namespace std;

Fenetre::Fenetre()
{

    QPushButton *ok=new QPushButton("calculer");
    QTextEdit* texte = new QTextEdit("entrer le sudoku");
    QVBoxLayout* fenetreLayout = new QVBoxLayout;
    fenetreLayout->addWidget(texte);
    fenetreLayout->addWidget(ok);

    this->setLayout(fenetreLayout);

    this->show();

    m_sudoku=NULL;
    m_afficherCalcul=NULL;
    m_afficherSolution=NULL;
    m_afficherHypothese=NULL;
    m_texte= texte;

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
}

void Fenetre::genererSolution(){
    string texte_sudoku = m_texte->toPlainText().toStdString();
    if (m_afficherCalcul)
        delete m_afficherCalcul;
    if( m_sudoku)
        delete m_sudoku;
    if (m_afficherSolution)
        delete m_afficherSolution;
    if (m_afficherHypothese)
        delete m_afficherHypothese;

    m_sudoku=new Essai(texte_sudoku);
    m_sudoku->iterer();
    string s_iterer = m_sudoku->getStringVal();

    m_sudoku->generer();
    vector<vector<int>> tableau(9,vector<int> (9,0));
    /*for(int i(0);i<9;++i)
        for(int j(0);j<9;++j)
            tableau[i][j]=m_sudoku->m_liste_cellules[i][j].m_valeur;*/
    m_sudoku->getSolution(tableau);

    string s_generer;
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

    string s_avant_solution;
    for(int j(0);j<9;++j){
        for(int i(0);i<9;++i)
            s_avant_solution += std::to_string(tableau2[i][j]);
        s_avant_solution +='\n';
    }

    Essai monSudokuTest(s_avant_solution);
    monSudokuTest.iterer();
    string s_solution= monSudokuTest.getStringVal();

    m_afficherCalcul = new QTextEdit;
    m_afficherCalcul->setPlainText(QString::fromStdString(s_iterer));
    m_afficherCalcul->setWindowTitle("premier calcul");
    m_afficherCalcul->show();

    m_afficherHypothese = new QTextEdit;
    m_afficherHypothese->setPlainText(QString::fromStdString(s_generer));
    m_afficherHypothese->setWindowTitle("hypotheses ajoutées");
    m_afficherHypothese->show();

    m_afficherSolution= new QTextEdit;
    m_afficherSolution->setPlainText(QString::fromStdString(s_solution));
    m_afficherSolution->setWindowTitle("solution calculée");
    m_afficherSolution->show();


}
