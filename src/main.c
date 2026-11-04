#include <stdio.h>
#include "grille.h"
#include "menu.h"


int main(void)
{
    screen_clear();
    // T_Case pion = {1, 1};

    // Affiche_Grille(10, 10, pion);

    afficher_menu();

    return 0;
}


// void Affiche_Grille(int L, int C, T_Case pion)
// {
//     for (int ligne = 1; ligne <= L; ligne++) {

//         for (int colonne = 1; colonne <= C; colonne++) {
//             printf("+---");
//         }
//         printf("+\n");

//         for (int colonne = 1; colonne <= C; colonne++) {
//             char symbole = ' ';

//             if (ligne == pion.ligne && colonne == pion.colonne) {
//                 symbole = 'P';
//             } else if (ligne == L && colonne == C) {
//                 symbole = 'X';
//             }

//             printf("| %c ", symbole);
//         }
//         printf("|\n");
//     }

//     for (int colonne = 1; colonne <= C; colonne++) {
//         printf("+---");
//     }
//     printf("+\n");

//     printf("P : pion | X : puits\n");
// }