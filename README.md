
**Programmation Unix : Jeu de société “En cavale”**

#####  Programmation  Système en C++ sous Linux, la Synchronisation et la Communication inter-Processus et les Signaux.

**I. Règles du jeu**

L’objectif du projet est de faire jouer deux processus au jeu de société Québécois appelé **“En cavale**” dont les règles sont expliquées sur le site de **Récréomath** :
[http://www.recreomath.qc.ca/jeu_21.htm](http://www.recreomath.qc.ca/jeu_21.htm)

Le nombre de jetons pourra être fixé à 18 ou déterminé dynamiquement à l’exécution **(entrée
clavier ou paramètre de l’exécutable).**
Même chose pour la taille de la grille qui peut être fixée à 6 × 6 ou déterminée dynamiquement
à l’exécution.
Des symboles (par exemple X, O) seront utilisés au lieu des couleurs.

**II. Spécifications techniques**

**Le projet a été codé en C++ sous Linux.**

**Le projet fait intervenir deux processus issus d’un même processus père qui jouent l’un
contre l’autre et qui communiquent grâce à un (ou plusieurs) tube(s).**
L’un des processus peut être celui de l’utilisateur (entrées clavier pour connaître ses actions),
l’autre peut jouer *automatiquement*.

Le mécanisme pour mettre en attente l’un des processus tant que l’autre n’a pas
fini de jouer suit la règle  suivante :
***“Si un processus lit dans un tube vide et si au moins un processus peut écrire dans ce tube,
alors le processus lecteur est bloqué tant que le tube reste vide.”***

Après chaque coup joué, il faut un mécanisme pour décider si la partie est finie ou non.
Si la partie est finie, les deux processus joueurs doivent se terminer eux-mêmes (exit( )) puis
leur père affiche les scores respectifs.

Le programme intègre le tout dans une boucle qui demande de rejouer une partie ou non et cumuler les scores de chaque partie.

Pour compiler (sous un environnement Linux):

    ./compile Jeu.cpp

Très bonne partie à vous !!!
