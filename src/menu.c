#include <stdio.h>
#include "menu.h"
#include "game.h"

#define TERM_LIGNES 24
#define TERM_COLONNES 80

#define DIMENSION_MIN 5
#define DIMENSION_MAX 50

void screen_clear(void)
{
    printf("\n");
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
    if (text == NULL) {
        return;
    }
    (void) row;
    (void) terminal_width;
    printf("%s\n", text);
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
    int caractere;

    printf("\nAppuyez sur Entree pour continuer...");
    fflush(stdout);
    do {
        caractere = getchar();
    // Ici EOF est utilisé pour les cas ou l'utilisateur ferme l'entrée standard (par exemple, en appuyant sur Ctrl+D sous Unix/Linux). Cela permet de sortir proprement de la boucle même si l'entrée est fermée.
    } while (caractere != '\n' && caractere != EOF);
}

static void vider_ligne(void)
{
    int caractere;

    do {
        caractere = getchar();
    } while (caractere != '\n' && caractere != EOF);
}

static int lire_entier(const char *message)
{
    int valeur;
    int resultat;

    while (1) {
        printf("%s", message);
        fflush(stdout);

        resultat = scanf("%d", &valeur);
        vider_ligne();

        if (resultat == 1) {
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
        } else if (choix != 2) {
            printf("Choix invalide.\n");
            attendre_entree();
        }
    } while (choix != 2);
}

void afficher_score(const T_Parametres *parametres)
{
    if (parametres == NULL) {
        return;
    }

    // Afficher le score de chaque joueur enregistré
    screen_clear();
    printf("===== SCORE =====\n\n");
    for (int i = 0; i < NOMBRE_JOUEURS_MAX; i++) {
        if (parametres->joueurs[i].nom[0] != '\0') {
            printf("%s : %d point%s\n",
                   parametres->joueurs[i].nom,
                   parametres->joueurs[i].points,
                   parametres->joueurs[i].points > 1 ? "s" : "");
        }
    }
    attendre_entree();
}

void afficher_menu(T_Parametres *parametres)
{
    const char *lignes[] = {
        "===== MENU =====",
        "1. Nouvelle partie",
        "2. Parametres",
        "3. Score",
        "4. Quitter"
    };
    const int nb_lignes = 5;
    int choix;
    int rows, cols;

    do {
        int premiere_ligne;

        screen_clear();
        if (get_terminal_size(&rows, &cols) == -1) {
            return;
        }

        premiere_ligne = (rows - (nb_lignes + 2)) / 2 + 1;
        // Afficher les différentes lignes du menu
        for (int i = 0; i < nb_lignes; i++) {
            screen_center_line(lignes[i], premiere_ligne + i, cols);
        }
        printf("\n");
        fflush(stdout);

        choix = lire_entier("Votre choix : ");

        switch (choix) {
            case 1:
                screen_clear();
                Start_Game(parametres);
                break;
            case 2:
                afficher_parametres(parametres);
                break;
            case 3:
                afficher_score(parametres);
                break;
            case 4:
                screen_clear();
                screen_middle("Au revoir !");
                printf("\n");
                break;
            default:
                screen_clear();
                screen_middle("Choix invalide.");
                printf("\n");
                attendre_entree();
        }
    } while (choix != 4);
}