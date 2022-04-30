#ifndef FENETRE_H
#define FENETRE_H

#include <QMainWindow>
#include <QTextEdit>
#include "sudoku.h"
#include <QButtonGroup>



class Fenetre : public QWidget
{
    Q_OBJECT

public :

    QWidget* m_fenetre;
    Essai* m_sudoku;
    Profondeur* m_profondeur;

    QTextEdit* m_afficherSolution;
    QTextEdit* m_afficherHypothese;
    QTextEdit* m_afficherCalcul;
    QTextEdit* m_texte;
    QTextEdit* m_nbrHypotheses;

    QButtonGroup* m_group;

    QWidget* m_fenetre_resultat;

public:
    Fenetre();
    ~Fenetre();

public slots :
    void genererSolution();
};

#endif // FENETRE_H
