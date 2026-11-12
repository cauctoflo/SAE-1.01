
#include <stdio.h>
#include "game.h"
#include "grille.h"
#include "menu.h"

const char *VERSION = "1.0.0";
const char *ELEVE1 = "FOULIGNY Florentin";
const char *ELEVE2 = "SOUVANLAVAH Leo";

void Start_Game(T_Parametres *parametres)
{
    int nb_joueurs;
    T_Case positions[NOMBRE_JOUEURS_MAX];

    if (parametres == NULL) {
        return;
    }

    screen_clear();
    screen_middle("===== BIENVENUE DANS LE JEU =====");
    printf("\n\nVersion : %s\n", VERSION);
    printf("Developpe par %s et %s\n", ELEVE1, ELEVE2);
    printf("Grille : %d lignes x %d colonnes\n",
           parametres->lignes, parametres->colonnes);
    printf("\nAppuyez sur Entree pour commencer...");
    getchar();

    // Demander le nombre de joueur
    printf("Combien de joueurs ? (1 à 4) : ");
    do {
        if (scanf("%d", &nb_joueurs) != 1) {
            nb_joueurs = 0;
            while (getchar() != '\n') {
            }
        }
    } while (nb_joueurs < 1 || nb_joueurs > 4);

    Game_Setup(&nb_joueurs, parametres);

    for (int i = 0; i < nb_joueurs; i++) {
        positions[i].ligne = 1;
        positions[i].colonne = i + 1;
    }

    while (getchar() != '\n') {
    }

    for (int i = 0; i < nb_joueurs; i++) {
        screen_clear();
        screen_middle("===== GRILLE DU JOUEUR =====");
        printf("\n\nJ%d : %s (%c)\n\n", i + 1,
               parametres->joueurs[i].nom,
               parametres->joueurs[i].symbole);
        Affiche_Grille(parametres->lignes, parametres->colonnes,
                       &positions[i], &parametres->joueurs[i], 1);
        printf("\nAppuyez sur Entree pour voir le joueur suivant...");
        getchar();
    }
}


void Game_Setup(int *Nb_Joueurs, T_Parametres *parametres)
{
    for (int i = 0; i < *Nb_Joueurs; i++) {
        printf("Nom du joueur %d : ", i + 1);
        if (scanf("%49s", parametres->joueurs[i].nom) != 1) {
            return;
        }

        printf("Symbole du joueur %d : ", i + 1);
        if (scanf(" %c", &parametres->joueurs[i].symbole) != 1) {
            return;
        }
    }
}

void Affiche_Grille(int L, int C, const T_Case positions[],
                    const Joueur joueurs[], int nombre_joueurs)
{
    for (int ligne = 1; ligne <= L; ligne++) {

        for (int colonne = 1; colonne <= C; colonne++) {
            printf("+---");
        }
        printf("+\n");

        for (int colonne = 1; colonne <= C; colonne++) {
            char symbole = ' ';

            for (int joueur = 0; joueur < nombre_joueurs; joueur++) {
                if (ligne == positions[joueur].ligne
                    && colonne == positions[joueur].colonne) {
                    symbole = joueurs[joueur].symbole;
                    break;
                }
            }
            if (symbole == ' ' && ligne == L && colonne == C) {
                symbole = 'X';
            }

            printf("| %c ", symbole);
        }
        printf("|\n");
    }

    for (int colonne = 1; colonne <= C; colonne++) {
        printf("+---");
    }
    printf("+\n");

    printf("X : puits\n");
}