#ifndef MENU_H
#define MENU_H

#include <stdio.h>

#define NOMBRE_JOUEURS_MAX 4
#define TAILLE_NOM_JOUEUR 50

typedef struct {
	char nom[TAILLE_NOM_JOUEUR];
	char symbole;
	int points;
	
} Joueur;

typedef struct {
	int lignes;
	int colonnes;
	int bas_max;
	int droite_max;
	Joueur joueurs[NOMBRE_JOUEURS_MAX];
} T_Parametres;



// FOnctions pour vider le buffer & l'écran
void screen_clear(void);

// Fonction pour afficher du texte au milieu de l'écran
void screen_middle(const char *text);

// Fonction pour afficher le menu principal
void afficher_menu(T_Parametres *parametres);

// Fonction pour avoir la taille du terminal
int get_terminal_size(int *rows, int *cols);

// Affiche le score de tout les joueurs
void afficher_score(const T_Parametres *parametres);

#endif



