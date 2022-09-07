#include "fenetre.h"
#include <QApplication>
#include <random>


/* CREE par GUSTAVE ROBICHON, sur Qt
 *  17/04/2021
 *  gustave.robichon23@gmail.com
 *
 *  programme original de résolution de sudoku
 *  calcule tous les sudokus possibles avec la matrice entrée,
 *  la liste est mélangée de maniere aléatoire
 *  s'arrête dès qu'une solution est trouvée : mémorise les hypothèses faites pour obtenir cette solution
 *  fait office de générateur de sudoku, si on entre un sudoku vide
 *
 *  enntrez un sudoku : 0 si le nombre est indéfini, [1,9] si il est défini, saut de ligne à la fin de chaque ligne :
 *  les autres caracteres sont neutres.
 *
 *  le temps nécessaires à créer une solution est long, car il y a N "for" : il y a un for écrit dans une fonction récursive.
 *  Cela explique que pour générer 10 hypotheses, il faut parcourir 10 fois un for qui parcourt les possibles ...
 *
 *  le temps d'exécution est rapide : quand une cellule change, on en informe aussi la ligne,
 *  et si la taille de la liste pour cette valeur vaut 1, on le voit directement !
 *  les fonctions s'appellent les une les autres en piles. Le raisonnement est bon, car :
 *  si on fait la modification A puis la B (la fonction A appelle B), le résultat est le même que pour B puis A :
 *  cela commute, et il n'y a pas d'erreurs.
 *
 *  normalement, pour chaque sudoku, il y a besoin d'un seul niveau d'essai pour trouver la solution unique.
 *  Si il y a plusieurs solutions, le programme suppose (crée) des hypotheses : un niveau supplémentaire par hypothèse.
 *  Heureusement le programme s'arrête quand les hypothèses suffisent à générer une solution unique : on ne parcourt pas les 10^100 possibilités !
 *
 *  l'interface est assez simple. On peut encore ajouter un compteur des hypothèses créées. c'est la fonction :
 *  getSolution qui renvoit aussi un int ...
 *
 *  la plus belle fonction, au centre des hypotheses testées, c'est la fonction enlever :
 *  si une hypothèse testée renvoit une erreur de sudoku, on l'enleve de l'essai mère, et aussi des autres essais à ce niveau.
 *  Et, si encore il y a une erreur dans un essai où on vient d'enlever (setOff), alors on enleve cet essai de la mère ET des filles.
 *
 *  Il y a setValeur et setOff, qui s'appellent : dans le sens carte, ligne-colonne-carré, cellule.
 *  Il y a une liste réciproque dans chaque ligne : pour chaque valeur il y a la liste des cellules ayant cette valeur comme possible.
 *  Si cette liste a la taille 1, on lance un setValeur. Si taille==0, on renvoit une erreur.
 *
 *  Chaque fonction renvoit un bool : controle des erreurs.
 *  Il y a un bool modifié : pour faire tourner les algorithmes de raisonnement : doublets, triples, les 3 du carrés sont tous sur la même ligne ...
 *
 * Un Test c'est une carte + hypotheses. Un essai c'est une carte avec une liste d'essais. il y a un pointeur vers la carte Mere.
 * Il manque à implémenter les 4-uplets (on pourrait même faire les n-uplets), mais avec un test d'hypthese on tombe sur le triplet qui,lui est géré.
 *
 * Quand on ouvre la carte, sans lancer les "raisonnements" (donc juste avec les setValeur), les sudokus faciles sont déjà résolus.
 * dans le raisonnement doublets : on chercher deux cases avec les deux mêmes valeurs (dans chaque case). On enleve ces deux valeurs possibles pour le reste du carré.0
 * dans le triplet, on cherche 3 cases qui en tout n'ont que 3 valeurs. 2 et 1 (valeurs) donnent une erreur.
 * dans le "calculerSi" on test si les 3 (par exemple) d'un carré sont sur la même ligne. Alors on enleve les 3 de cette ligne qui ne sont pas dans le carré. Idem avec lignes et colonnes.
 *
 * Je ne connais pas d'autre raisonnement, à part avec les hypotheses.
 * L'itération de niveauDeux est proposée.
 * Le nombre (majorant) de solutions est aussi proposé.
 *
 * Note : les numéros des carrés vont de 0 à 8. numero%3 va de 0 à 2 (en x), numero/3 va de 0 à 2 (en y). x + 3*y est son numéro. Ce n'était pas là une erreur de calcul.
 * On pourrait accélérer le temps de convergence pour la création d'hypothèse : car pour chaque niveau on prend toutes les hypothèses. Mais au moins ce calcul là était total ...
 *
 * j'utilise des continue et break : passer à l'étape n+1 du for; et quitter la boucle. Je n'ai pas trouvé d'erreur dans mon code ...
 *
 *
 * Si il reste des questions, n'hésitez pas à m'écrire.
 * Ce qui est beau c'est les fonctions qui s'appellent les une les autres, plutôt que des boucles directes, qu'on a logiquement été obligé d'expliciter pour les "raisonnements".
 * "Essai::Enlever" est donc la plus belle :-) avec les setValeur et setOff qui s'appellent les unes les autres ...
 * 
 * Bonne lecture !
 *
 * GUSTAVE ROBICHON
*/

int main(int argc, char *argv[])
{
    std::srand(std::time(nullptr));

    QApplication a(argc, argv);
    Fenetre w;

    w.show();

    return a.exec();
}
