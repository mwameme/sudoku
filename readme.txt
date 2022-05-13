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

<<<<<<<<<<<<<<<<<< Résumé : >>>>>>>>>>>>>>>>>>

liste et liste réciproques : liste dans une case des chiffres encore possibles, et liste dans un carré, pour chaque chiffre, des cases qui acceptent encore ce chiffre-là. Ca marche en récursif

Chercher les doublets : 2 cases dans le même carré qui on toutes les deux seulement deux chiffres (et les mêmes). Enlever ces 2 possibilités des autres cellules du même carré

Idem : triplet

raisonnement - Si : si dans un carré les 3 sont tous dans la ligne du bas (on enlève les 3 de cette ligne, sauf dans ce même carré).

Puis les niveaux : dans la carte de base on fait la liste des numéros encore possibles, on les teste chacun, et à chaque erreur on enlève le numéro : de la carte de base, et des essais qu'on est en train de faire.

Niveau : profondeur : on ne test pas tout le niveau 1 : on prend un des nombres possibles au hasard, et on teste ... Génère plus d' "hypothèses"
