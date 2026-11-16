#ifndef GAME_H
#define GAME_H

extern const char *VERSION;
extern const char *ELEVE1;
extern const char *ELEVE2;

#include "menu.h"
#include "pion.h"

// Fonction pour démarrer le jeu
void Start_Game(T_Parametres *parametres);
// Fonction pour configurer le jeu (demander les noms et symboles des joueurs)
void Game_Setup(int *Nb_Joueurs, T_Parametres *parametres);
// Fonction pour proposer et jouer un coup valide
int Coup_Joueur(T_Case *position, const T_Parametres *parametres);
// Fonction pour faire gagner le joueur actuel
void Win_Game(Joueur *joueur);

#endif