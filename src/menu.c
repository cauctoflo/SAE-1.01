#include <stdio.h>
#include <string.h>
// Bibliothèque pour obtenir la taille du terminal
#include <sys/ioctl.h>

// Bibliothèque pour obtenir la taille du terminal
#include <unistd.h>
#include "menu.h"

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
void afficher_menu(void)
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

        scanf("%d", &choix);

        switch (choix) {
            case 1:
                screen_middle("Nouvelle partie...\n");
                break;

            case 2:
                screen_middle("Parametres...\n");
                break;

            case 3:
                screen_middle("Au revoir !\n");
                break;

            default:
                screen_middle("Choix invalide.\n");
        }

    } while (choix != 3);
}