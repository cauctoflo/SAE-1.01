#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "menu.h"

#define TERM_LIGNES 24
#define TERM_COLONNES 80

#define DIMENSION_MIN 5
#define DIMENSION_MAX 50

#define LECTURE_ERREUR INT_MIN

void screen_clear(void)
{
    printf("\033[2J\033[H");
    fflush(stdout);
}

int get_terminal_size(int *rows, int *cols)
{
    if (rows == NULL || cols == NULL) {
        return -1;
    }
    *rows = TERM_LIGNES;
    *cols = TERM_COLONNES;
    return 0;
}

static void screen_center_line(const char *text, int row, int terminal_width)
{
    int column;

    if (text == NULL) {
        return;
    }
    column = (terminal_width - (int) strlen(text)) / 2;
    if (column < 0) {
        column = 0;
    }
    printf("\033[%d;%dH%s", row, column + 1, text);
}

void screen_middle(const char *text)
{
    int rows, cols;

    if (get_terminal_size(&rows, &cols) == -1) {
        return;
    }
    screen_center_line(text, (rows + 1) / 2, cols);
    fflush(stdout);
}

static void attendre_entree(void)
{
    char ligne[64];

    printf("\nAppuyez sur Entree pour continuer...");
    fflush(stdout);
    fgets(ligne, sizeof(ligne), stdin);
}

static int lire_entier(const char *message)
{
    char ligne[64];
    int valeur;
    char reste;

    while (1) {
        printf("%s", message);
        fflush(stdout);

        if (fgets(ligne, sizeof(ligne), stdin) == NULL) {
            return LECTURE_ERREUR;
        }
        if (sscanf(ligne, "%d %c", &valeur, &reste) == 1) {
            return valeur;
        }
        printf("Veuillez saisir un nombre entier.\n");
    }
}

static int parametres_valides(const T_Parametres *p)
{
    if (p->lignes < DIMENSION_MIN || p->lignes > DIMENSION_MAX) {
        return 0;
    }
    if (p->colonnes < DIMENSION_MIN || p->colonnes > DIMENSION_MAX) {
        return 0;
    }
    if (p->bas_max < 1 || p->bas_max > p->lignes / 2) {
        return 0;
    }
    if (p->droite_max < 1 || p->droite_max > p->colonnes / 2) {
        return 0;
    }
    if (p->bas_max + p->droite_max > 9) {
        return 0;
    }
    return 1;
}

static void afficher_parametres(T_Parametres *parametres)
{
    int choix;

    do {
        screen_clear();
        printf("===== PARAMETRES =====\n\n");
        printf("Grille : %d lignes x %d colonnes\n",
               parametres->lignes, parametres->colonnes);
        printf("Deplacement maximum vers le bas : %d\n", parametres->bas_max);
        printf("Deplacement maximum vers la droite : %d\n\n",
               parametres->droite_max);
        printf("1. Modifier les parametres\n");
        printf("2. Retour\n\n");

        choix = lire_entier("Votre choix : ");

        if (choix == 1) {
            T_Parametres nouveaux = *parametres;

            nouveaux.lignes = lire_entier("Nombre de lignes (5 a 50) : ");
            nouveaux.colonnes = lire_entier("Nombre de colonnes (5 a 50) : ");
            nouveaux.bas_max = lire_entier("Deplacement maximum vers le bas : ");
            nouveaux.droite_max = lire_entier("Deplacement maximum vers la droite : ");

            if (parametres_valides(&nouveaux)) {
                *parametres = nouveaux;
                printf("Parametres enregistres.\n");
            } else {
                printf("Parametres invalides : verifiez les limites et b + d <= 9.\n");
            }
            attendre_entree();
        } else if (choix != 2 && choix != LECTURE_ERREUR) {
            printf("Choix invalide.\n");
            attendre_entree();
        }
    } while (choix != 2 && choix != LECTURE_ERREUR);
}

void afficher_menu(T_Parametres *parametres)
{
    const char *lignes[] = {
        "===== MENU =====",
        "1. Nouvelle partie",
        "2. Parametres",
        "3. Quitter"
    };
    const int nb_lignes = 4;
    int choix;
    int rows, cols;

    do {
        int premiere_ligne;

        screen_clear();
        if (get_terminal_size(&rows, &cols) == -1) {
            return;
        }

        premiere_ligne = (rows - (nb_lignes + 2)) / 2 + 1;
        for (int i = 0; i < nb_lignes; i++) {
            screen_center_line(lignes[i], premiere_ligne + i, cols);
        }
        printf("\033[%d;%dH", premiere_ligne + nb_lignes + 1,
               (cols - 14) / 2 + 1);
        fflush(stdout);

        choix = lire_entier("Votre choix : ");

        switch (choix) {
            case 1:
                screen_clear();
                screen_middle("Nouvelle partie...");
                printf("\033[%d;1H", rows);
                attendre_entree();
                break;
            case 2:
                afficher_parametres(parametres);
                break;
            case 3:
                screen_clear();
                screen_middle("Au revoir !");
                printf("\033[%d;1H\n", rows);
                break;
            case LECTURE_ERREUR:
                break;
            default:
                screen_clear();
                screen_middle("Choix invalide.");
                printf("\033[%d;1H", rows);
                attendre_entree();
        }
    } while (choix != 3 && choix != LECTURE_ERREUR);
}