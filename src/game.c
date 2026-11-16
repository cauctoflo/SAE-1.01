
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "grille.h"
#include "menu.h"

const char *VERSION = "1.0.0";
const char *ELEVE1 = "FOULIGNY Florentin";
const char *ELEVE2 = "SOUVANLAVAH Leo";

static void vider_ligne(void)
{
    int caractere;

    do {
        caractere = getchar();
    } while (caractere != '\n' && caractere != EOF);
}

static int choisir_joueur_depart(void)
{
    int choix;
    int joueur;

    // Proposer les deux façons de choisir le premier joueur
    do {
        printf("\nQui commence ?\n");
        printf("1. Choisir aleatoirement\n");
        printf("2. Choisir le joueur\n");
        printf("Votre choix : ");

        if (scanf("%d", &choix) != 1) {
            choix = 0;
        }
        vider_ligne();
    } while (choix < 1 || choix > 2);

    // Choisir le premier joueur au hasard
    if (choix == 1) {
        srand((unsigned int) time(NULL));
        joueur = rand() % 2;
        printf("Le joueur %d commence.\n", joueur + 1);
        return joueur;
    }

    // Demander directement quel joueur doit commencer
    do {
        printf("Quel joueur commence ? (1 ou 2) : ");
        if (scanf("%d", &joueur) != 1) {
            joueur = 0;
        }
        vider_ligne();
    } while (joueur < 1 || joueur > 2);

    return joueur - 1;
}

static int demander_rejouer(void)
{
    int choix;

    // Proposer de refaire une partie avec les mêmes joueurs ou de les changer
    do {
        printf("\nVoulez-vous relancer une partie ?\n");
        printf("1. Oui, avec les mêmes joueurs\n");
        printf("2. Oui, avec de nouveaux joueurs\n");
        printf("3. Non, retourner au menu\n");
        printf("Votre choix : ");

        if (scanf("%d", &choix) != 1) {
            choix = 0;
        }
        vider_ligne();
    } while (choix < 1 || choix > 3);

    return choix;
}

void Start_Game(T_Parametres *parametres)
{
    int nb_joueurs;
    T_Case position;

    if (parametres == NULL) {
        return;
    }

    // Afficher les informations de la partie avant de commencer
    screen_clear();
    screen_middle("===== BIENVENUE DANS LE JEU =====");
    printf("\n\nVersion : %s\n", VERSION);
    printf("Developpe par %s et %s\n", ELEVE1, ELEVE2);
    printf("Grille : %d lignes x %d colonnes\n",
           parametres->lignes, parametres->colonnes);
    printf("\nAppuyez sur Entree pour commencer...");
    getchar();

    // Le jeu oppose deux joueurs
    nb_joueurs = 2;

    Game_Setup(&nb_joueurs, parametres);
    while (getchar() != '\n') {
    }

    int rejouer;
    do {
        // Initialiser le pion au début de chaque partie
        position.ligne = 1;
        position.colonne = 1;

        // Choisir le joueur qui commence la partie
        int joueur_actuel = choisir_joueur_depart();

        // Répéter les tours jusqu'à ce qu'un joueur atteigne le puits
        while (1) {
            screen_clear();
            screen_middle("===== GRILLE DU JOUEUR =====");

            printf("\nAu tour de: %s\n",
                   parametres->joueurs[joueur_actuel].nom);
            printf("Symbole: %c\n",
                   parametres->joueurs[joueur_actuel].symbole);

            // Afficher la grille avec la position actuelle du pion
            Affiche_Grille(parametres->lignes, parametres->colonnes, &position);

            // Demander au joueur actuel de choisir et de jouer un coup valide
            Coup_Joueur(&position, parametres);

            // Vérifier si le joueur a atteint le puits
            if (position.ligne == parametres->lignes
                && position.colonne == parametres->colonnes) {
                Win_Game(&parametres->joueurs[joueur_actuel]);
                break;
            }

            printf("Appuyez sur Entree pour continuer...");
            getchar();
            // Passer au joueur suivant après chaque coup
            joueur_actuel = (joueur_actuel + 1) % nb_joueurs;
        }

        rejouer = demander_rejouer();

        if (rejouer == 2) {
            // Ressaisir les joueurs pour la prochaine partie
            Game_Setup(&nb_joueurs, parametres);
            while (getchar() != '\n') {
            }
        }
    } while (rejouer != 3);
}

int Coup_Joueur(T_Case *position, const T_Parametres *parametres)
{
    int coups_droite;
    int coups_bas;
    int nombre_coups = 0;
    int choix;

    // Vérifier que les paramètres nécessaires sont valides
    if (position == NULL || parametres == NULL) {
        return 0;
    }

    // Calculer le nombre de cases accessibles vers la droite
    coups_droite = parametres->colonnes - position->colonne;
    if (coups_droite > parametres->droite_max) {
        coups_droite = parametres->droite_max;
    }

    // Calculer le nombre de cases accessibles vers le bas
    coups_bas = parametres->lignes - position->ligne;
    if (coups_bas > parametres->bas_max) {
        coups_bas = parametres->bas_max;
    }

    // Afficher tous les coups possibles et les numéroter
    printf("\nCoups possibles :\n");
    for (int deplacement = 1; deplacement <= coups_droite; deplacement++) {
        nombre_coups++;
        printf("%d. droite de %d case%s\n", nombre_coups, deplacement,
               deplacement > 1 ? "s" : "");
    }
    for (int deplacement = 1; deplacement <= coups_bas; deplacement++) {
        nombre_coups++;
        printf("%d. bas de %d case%s\n", nombre_coups, deplacement,
               deplacement > 1 ? "s" : "");
    }

    if (nombre_coups == 0) {
        return 0;
    }

    // Demander un choix jusqu'à ce que le numéro soit valide
    do {
        printf("Choisissez un coup (1-%d) : ", nombre_coups);
        if (scanf("%d", &choix) != 1) {
            choix = 0;
        }
        vider_ligne();
        if (choix < 1 || choix > nombre_coups) {
            printf("Coup invalide.\n");
        }
    } while (choix < 1 || choix > nombre_coups);

    // Appliquer le déplacement choisi à la position du joueur
    if (choix <= coups_droite) {
        position->colonne += choix;
    } else {
        position->ligne += choix - coups_droite;
    }

    return 1;
}


void Game_Setup(int *Nb_Joueurs, T_Parametres *parametres)
{
    // Demander les informations des deux joueurs
    for (int i = 0; i < *Nb_Joueurs; i++) {
        printf("Nom du joueur %d : ", i + 1);
        if (scanf("%49s", parametres->joueurs[i].nom) != 1) {
            return;
        }

        printf("Symbole du joueur %d : ", i + 1);
        if (scanf(" %c", &parametres->joueurs[i].symbole) != 1) {
            return;
        }
        parametres->joueurs[i].points = 0;
    }
}

void Affiche_Grille(int L, int C, const T_Case *position)
{
    // Parcourir les lignes et les colonnes sans stocker la grille en mémoire
    for (int ligne = 1; ligne <= L; ligne++) {

        // Afficher la bordure supérieure de chaque ligne
        for (int colonne = 1; colonne <= C; colonne++) {
            printf("+---");
        }
        printf("+\n");

        for (int colonne = 1; colonne <= C; colonne++) {
            char symbole = ' ';

            // Le pion est unique et sa position est partagee par les joueurs
            if (position != NULL && ligne == position->ligne
                && colonne == position->colonne) {
                symbole = 'P';
            }
            if (symbole == ' ' && ligne == L && colonne == C) {
                // La dernière case représente le puits
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

void Win_Game(Joueur *joueur)
{
    // Afficher le gagnant et augmenter son score
    printf("\nFélicitations %s ! Vous avez atteint le puits et gagné la partie !\n", joueur->nom);
    joueur->points++; // Incrémente le score du joueur
}