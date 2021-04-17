#ifndef FENETRE_H
#define FENETRE_H

#include <QMainWindow>
#include <QTextEdit>
#include "sudoku.h"


class Fenetre : public QWidget
{
    Q_OBJECT

public :

    QWidget* m_fenetre;
    Essai* m_sudoku;
    QTextEdit* m_afficherSolution;
    QTextEdit* m_afficherHypothese;
    QTextEdit* m_afficherCalcul;
    QTextEdit* m_texte;

public:
    Fenetre();
    ~Fenetre();

public slots :
    void genererSolution();
};

#endif // FENETRE_H
