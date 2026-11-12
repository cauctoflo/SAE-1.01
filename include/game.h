#ifndef GAME_H
#define GAME_H

extern const char *VERSION;
extern const char *ELEVE1;
extern const char *ELEVE2;

#include "menu.h"

void Start_Game(T_Parametres *parametres);
void Game_Setup(int *Nb_Joueurs, T_Parametres *parametres);

#endif