<<<<<<< HEAD:sudoku Qt/main.cpp
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
=======
Utilisation :

il y a 3 modes de génération de sudoku, pour un seul premier calcul. Le premier calcul résoud le sudoku, et si il n'est pas encore résolu, l'un des trois modes de génération ajoute des "hypothèses" afin qu'il n'y ait qu'une solution.

Vous pouvez donc choisir un des trois modes de résolution : "niveau un : minimum", "niveau un : aléatoire", "profondeur : aléatoire". Pour générez un sudoku, cliquez directement sur "résoudre", ou bien videz la fenetre puis cliquez sur "résoudre".

Pour trouver la solution d'un sudoku (trouvé sur internet par exemple), recopiez le (sans saut de ligne au début) en mettant des 0 lorsque le nombre n'est pas connu, et en sautant une ligne à la fin de chaque ligne. Puis cliquez sur "résoudre". Vous pouvez ainsi aussi recopier les "hypothèses" générées par le générateur de sudoku dans la fenetre de début. Atttention aux sauts de lignes !

Premier calcul :
il y a trois algorithmes : si dans une case, il n'y a plus qu'une seule possibilité, alors on enlève cette possibilité des autres cases de la même ligne, de la même colonne, du même carré. Idem, si dans un carré (ou une ligne ou une colonne) il n'y a plus qu'une seule case pour avoir le "2", on valide le "2" pour cette case, dans cette ligne dans cette colonne et dans ce carré. Celà est écrit de manière récursive : à chaque fois qu'on modifie une case (ou une ligne ou une colonne ou un carré) on vérifie + modifie les autres listes de la même case. 
deuxième algorithme : si (disons dans un carré) il y a deux cases (des doublets) qui contiennet chacun seulement deux chiffres différents (2 et 4), alors on enlève le 2 et le 4 des autres cases du même carré. Idem pour ligne et colonne.
troisième algorithme :
si (disons dans un carré), les 3 sont uniquement sur la ligne du bas, on enlève les 3 de la ligne du bas sauf dans le même carré. Idem pour les autres objets : si les 3 sont dans une ligne uniquement dans un carré, alors on enlève les 3 du carré sauf dans cette même ligne.
Ces 3 algorithmes sont tournés en boucle, tant qu'il y a une modification quelconque

Générateur : niveau un : minimum
On commence par le premier calcul, et on suppose qu'il n'a pas terminé la résolution. Alors on prend la liste des nombres (localisés en une ligne et une colonne) encore possibles : c'est le niveau un. On recopie la carte initiale pour chacune de ces possibilités, et on test chacune des ces possibilités (dans la copie de la carte). On sélectionne le test qui a enlevé le plus de nombres possibles, (on oublie les autres essais), et on recommence : on regarde les nombres encore possibles, on recopie (cette deuxieme carte), on génère le "niveau un", on regarde l'essai qui enleve le plus de possibilités, etc ...
On remarque que lorsqu'un essai (au-dessus d'une carte) induit un résultat faux, on enleve cette possibilité de la carte (juste avant). Par exemple je croyais que le 2 en ligne 3 et colonne 3 était possible, je le teste, et je trouve une erreur dans la carte : alors j'enleve cette possibilité de la carte, mais donc aussi de tout le "niveau un" au-dessus de cette carte.
Cette génération est belle, justement parcequ'elle enlève les erreurs (les essais révélés impossibles), et donc génère un nombre minimum d'hypothèses. C'était beau à programmer mais un peu lent à l'exécution.

Générateur : niveau un : aléatoire
C'est exactement le même algorithme que le précédent, sauf qu'on choisit l'essai au hasard (parmi les essais générés au niveau un). Bien sûr on tombe sur un nombre d'hypothèses plus grand que précédemment, mais l'aléa permet de générer des sudokus différents

Générateur : profondeur : aléatoire
dans cet algorithme, on prend la carte de base, on effectue le premier calcul. Puis on prend un des nombres encore possibles (au hasard), et on le test : on recopie la carte, et dans la carte on force ce nombre à être vrai (dans la case définie, évidemment). Si ce test génère un erreur, on enlève ce nombre de la carte de base. On relance le "premier calcul" (pour nettoyer ce qui peut l'être). Si il n'y a pas d'erreur immédiate, on reprend un nombre au hasard (dans cette deuxieme carte), on le test, on regarde si il y a une erreur. Donc on test les hypothèses d'abord en profondeur. Si essai1 -> essai2 -> essai3, et que dans essai3 il y a une erreur, on redescend à essai2, on y enlève test3 de essai2, et si il y a une erreur dans essai2 on redescend à essai1, sinon on crée essai3bis, etc ... On monte et on descend.
Dans le "niveau un", on part de essai, on crée la liste essai1, on enlève ce qui donne un résultat faux de essai et de la liste essai1, on choisit un élément au hasard de essai1, et on génère essai2, etc ...
Profondeur calcule beaucoup plus vite que "niveau un", mais à chaque fois génère plus d'hypothèses


<<<<< Programmation : C++. Les classes utilisées. >>>>>

Cellule : contient ses coordonnées (ligne et colonne), sa valeur si elle est définie, et la liste de ses choix encore possibles
Ligne : contient sa liste des cellules, mais aussi la liste réciproque : pour "1" il y a la liste des cellules avec "1", idem pour "2", etc  ... Celà permet d'être plus rapide (reconnaitre directement que "1" n'a qu'une seule possibilité). La liste des cellules est sous forme de pointeur  (si on modifie une cellule dans ligne, ça la modifie ailleurs)
Colonne, Carré : idem ligne
Carte : la liste des cellules, liste des lignes, liste des carrés, liste des colonnes. Contient un booléen "modification" pour savoir si on a modifié qqchose.

niveau un;
Test : est le même objet que carte, avec aussi coordonnées + valeur (on copie une carte dans un test, et on essaye le nombre + coordonnées).
Essai : c'est un test, avec une liste d'essai. Il y a un pointeur vers l'essaai solution (pour savoir lequel des essais on est en train de tester, afin que si on a trouvé une solution on puisse la retrouver). 
dans Essai : on peut rajouter les nombres encore possibles : on crée la liste d'essais. A chaque fois que l'un est faux, on l'enlève de essai (qui est un test donc une carte), et on l'enlève de liste des essais. Si en faisant ça, l'essai actuel devient faux, on renvoit une erreur et donc on descend au niveau -1. Si on tombe sur un résultat final, on renvoit "true". La gestion des erreurs fait que si un test renvoit true on renvoit encore true, et si il renvoit false on enlève l'essai des possibles, et si ce faisant il y a une erreur on renvoit false.
Renvoyer fait descendre au niveau n-1. 
On peut aussi nettoyer la liste : si dans l'essai x, y est trouvé comme true, et que dans l'essai y, x est trouvé comme true, les deux représentent la même carte.

profondeur;
La class profondeur est une carte, avec un test, et un pointeur vers une autre profondeur (niveau n+1). On choisit un test au hasard, on copie la carte, on la test, et si false on l'enlève, si le résultat est total on renvoit true, si on récupère true on renvoit true. Il y a seulement 3 fonctions :
choisir un possible au hasard
essayer (regarder, lorsqu'on teste, si il y a une erreur). 
boucle : générer n+1

On remarque que le niveau n+1 se calcule en définissant dans un objet un pointeur vers un objet. Dans une fonction, on met une boucle, et dans la boucle on appelle la fonction (associée à l'objet pointé) : au niveau 3 celà donne une boucle de boucle de boucle. Donc celà génère un calcul de niveau (ici 3). Si dans une boucle on trouve un false, on quitte la boucle (donc on descend de niveau). Si on trouve un true on quitte toutes les boucles : c'est la solution.

Cet algorithme est donc juste un calcul de niveau, au-dessus d'un premier calcul (de sudoku).
>>>>>>> ff0b28bcf1a7458dfccc79e85c4c742aa0d607aa:README.md
