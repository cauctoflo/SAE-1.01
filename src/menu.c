#include <stdio.h>
#include <string.h>
// Bibliothèque pour obtenir la taille du terminal
#include <sys/ioctl.h>

// Bibliothèque pour obtenir la taille du terminal
#include <unistd.h>
#include "menu.h"

#define DIMENSION_MIN 5
#define DIMENSION_MAX 50

static int lire_entier(const char *message)
{
    char ligne[64];
    int valeur;
    char caractere;

    while (1) {
        printf("%s", message);
        fflush(stdout);

        if (fgets(ligne, sizeof(ligne), stdin) == NULL) {
            return -1;
        }

        if (sscanf(ligne, "%d %c", &valeur, &caractere) == 1) {
            return valeur;
        }

        printf("Veuillez saisir un nombre entier.\n");
    }
}

static void afficher_parametres(T_Parametres *parametres)
{
    int choix;

    do {
        screen_clear();
        printf("===== PARAMETRES =====\n\n");
        printf("Grille : %d lignes x %d colonnes\n", parametres->lignes,
               parametres->colonnes);
        printf("Deplacement maximum vers le bas : %d\n", parametres->bas_max);
        printf("Deplacement maximum vers la droite : %d\n\n",
               parametres->droite_max);
        printf("1. Modifier les parametres\n");
        printf("2. Retour\n");

        choix = lire_entier("Votre choix : ");

        if (choix == 1) {
            T_Parametres nouveaux = *parametres;

            nouveaux.lignes = lire_entier("Nombre de lignes (5 a 50) : ");
            nouveaux.colonnes = lire_entier("Nombre de colonnes (5 a 50) : ");
            nouveaux.bas_max = lire_entier("Deplacement maximum vers le bas : ");
            nouveaux.droite_max = lire_entier("Deplacement maximum vers la droite : ");

            if (nouveaux.lignes < DIMENSION_MIN ||
                nouveaux.lignes > DIMENSION_MAX ||
                nouveaux.colonnes < DIMENSION_MIN ||
                nouveaux.colonnes > DIMENSION_MAX ||
                nouveaux.bas_max < 1 ||
                nouveaux.bas_max > nouveaux.lignes / 2 ||
                nouveaux.droite_max < 1 ||
                nouveaux.droite_max > nouveaux.colonnes / 2 ||
                nouveaux.bas_max + nouveaux.droite_max > 9) {
                printf("Parametres invalides : verifiez les limites et b + d <= 9.\n");
                printf("Appuyez sur Entree pour continuer...");
                getchar();
            } else {
                *parametres = nouveaux;
                printf("Parametres enregistres.\n");
                printf("Appuyez sur Entree pour continuer...");
                getchar();
            }
        } else if (choix != 2 && choix != -1) {
            printf("Choix invalide.\n");
            printf("Appuyez sur Entree pour continuer...");
            getchar();
        }
    } while (choix != 2 && choix != -1);
}

void screen_clear(void) 
{
    printf("\033[2J\033[H");
    fflush(stdout);
}

int get_terminal_size(int *rows, int *cols) 
{
    struct winsize terminal;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal) == -1) {
        return -1; // Erreur lors de l'obtention de la taille du terminal
    }
    *rows = terminal.ws_row;
    *cols = terminal.ws_col;
    return 0; // Succès
}

// Fonction pour centrer le texte sur une ligne spécifique
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
// Fonction pour afficher du texte au milieu de l'écran
void screen_middle(const char *text)
{
    struct winsize terminal;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal) == -1) {
        return;
    }
    // On utilise (terminal.ws_row + 1) / 2 pour centrer le texte verticalement
    screen_center_line(text, ((int) terminal.ws_row + 1) / 2, (int) terminal.ws_col);
    fflush(stdout);
}

// La fonction pour afficher le menu principal du jeu
void afficher_menu(T_Parametres *parametres)
{
    int choix = 0;
    const char *lignes[] = {
        "===== MENU =====",
        "1. Nouvelle partie",
        "2. Parametres",
        "3. Quitter",
        "Votre choix : "
    };

    do {
        struct winsize terminal;

        screen_clear();
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal) != -1) {
            int premiere_ligne = ((int) terminal.ws_row - 5) / 2 + 1;

            for (int i = 0; i < 5; i++) {
                screen_center_line(lignes[i], premiere_ligne + i, (int) terminal.ws_col);
            }
            fflush(stdout);
        }

        choix = lire_entier("Votre choix : ");

        switch (choix) {
            case 1:
                screen_middle("Nouvelle partie...\n");
                break;

            case 2:
                afficher_parametres(parametres);
                break;

            case 3:
                screen_middle("Au revoir !\n");
                break;

            default:
                screen_middle("Choix invalide.\n");
        }

    } while (choix != 3 && choix != -1);
    screen_clear();
}