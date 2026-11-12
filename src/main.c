#include <stdio.h>
#include "grille.h"
#include "menu.h"

int main(void)
{
    screen_clear();
    // Initialisation des paramètres du jeu
    // Pion du joueur

    // Paramètres du jeu
    T_Parametres parametres;
    parametres.lignes = 10;
    parametres.colonnes = 10;
    parametres.bas_max = 2;
    parametres.droite_max = 2;

    // parametres.j1->nom = "Joueur 1";
    // parametres.j1->symbole = 'X';

    // parametres.j2->nom = "Joueur 2";
    // parametres.j2->symbole = 'O';

    
    screen_clear();
    afficher_menu(&parametres);

    return 0;
}
