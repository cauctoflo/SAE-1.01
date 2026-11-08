#ifndef MENU_H
#define MENU_H

#include <stdio.h>

typedef struct {
	int lignes;
	int colonnes;
	int bas_max;
	int droite_max;
} T_Parametres;

// FOnctions pour vider le buffer & l'écran
void screen_clear(void);

// Fonction pour afficher du texte au milieu de l'écran
void screen_middle(const char *text);

// Fonction pour afficher le menu principal
void afficher_menu(T_Parametres *parametres);

// Fonction pour avoir la taille du terminal
int get_terminal_size(int *rows, int *cols);

#endif