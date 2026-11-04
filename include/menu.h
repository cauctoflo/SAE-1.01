#ifndef MENU_H
#define MENU_H

#include <stdio.h>

// FOnctions pour vider le buffer & l'écran
void screen_clear(void);

// Fonction pour afficher du texte au milieu de l'écran
void screen_middle(const char *text);

// Fonction pour afficher le menu principal
void afficher_menu(void);

// Fonction pour avoir la taille du terminal
int get_terminal_size(int *rows, int *cols);

#endif