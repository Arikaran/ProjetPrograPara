#Groupe
Samuel BIJOU
Thomas ALLARD
Chris GAGOUDE

#To run  : /* To compile : 
		cd src
		cmake ..
		make
# To run : ./main 4 e061428097b79aab3d587dbb523e19f4e58b67346699208c81f8b5a9449ffe6f


# ProjetPrograPara
Projet de Programmation & Parallélisme
--------------------------------------

### Présentation du projet
Les ordinateurs d'aujourd'hui sont composés d'un grand nombres de cœurs de calculs. Les attaques par
force-brute font partie des techniques les plus employés par les pirates informatiques.
Le projet consiste à développer une application capable d’opérer une attaque par force-brute afin de
trouver la valeur d'une clé de hachage SHA 256.


### Objectif
L'objectif du projet est de comprendre l'étendu des problématiques qui peuvent être rencontrer lors de la
réalisation d'un logicielle parallèle, ainsi que d'adopter une stratégie pour la parallélisation du logiciel.
Une partie théorique est également à mettre en application (loi d'Amdahl).


### Fonctionnement
Le programme devra prendre deux arguments en entrée:
 * Une empreinte hexadécimale chiffrée de la phrase (ou du mot) à retouver
 * Le nombre de threads à lancer en parallèle (une valeur par défaut est affectée si l'utilisateur n'en donne pas)

L'application devra trouver, par la méthode de force-brute, à quel mot correspond l'empreinte fournie en entrée
et le retourner à l'utilisateur.
