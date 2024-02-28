#ifndef FENETRE_H
#define FENETRE_H

#include <QMainWindow>
#include <QTextEdit>
#include "sudoku.h"
#include <QButtonGroup>
#include <QDoubleSpinBox>

#include <exception>

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
    QTextEdit* m_afficherNbrHypotheses;
    QDoubleSpinBox* m_entree_temperature;

    QButtonGroup* m_group;

    QWidget* m_fenetre_resultat;


    bool m_erreur_carte;
public:
    Fenetre();
    ~Fenetre();

public slots :
    void genererSolution();
};

#endif // FENETRE_H
