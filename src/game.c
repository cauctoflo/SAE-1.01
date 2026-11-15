
#include <stdio.h>
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

static int lire_deplacement(int maximum)
{
    int deplacement;

    do {
        printf("Combien de cases voulez-vous avancer ? (Max : %d) : ",
               maximum);
        if (scanf("%d", &deplacement) != 1) {
            deplacement = 0;
        }
        vider_ligne();
    } while (deplacement < 1 || deplacement > maximum);

    return deplacement;
}

void Start_Game(T_Parametres *parametres)
{
    int nb_joueurs;
    T_Case positions[NOMBRE_JOUEURS_MAX];

    if (parametres == NULL) {
        return;
    }

    screen_clear();
    screen_middle("===== BIENVENUE DANS LE JEU =====");
    printf("\n\nVersion : %s\n", VERSION);
    printf("Developpe par %s et %s\n", ELEVE1, ELEVE2);
    printf("Grille : %d lignes x %d colonnes\n",
           parametres->lignes, parametres->colonnes);
    printf("\nAppuyez sur Entree pour commencer...");
    getchar();

    // Demander le nombre de joueur
    printf("Combien de joueurs ? (1 à 4) : ");
    do {
        if (scanf("%d", &nb_joueurs) != 1) {
            nb_joueurs = 0;
            while (getchar() != '\n') {
            }
        }
    } while (nb_joueurs < 1 || nb_joueurs > 4);

    Game_Setup(&nb_joueurs, parametres);

    // Initialiser les positions des joueurs au début de la partie
    for (int i = 0; i < nb_joueurs; i++) {
        positions[i].ligne = 1;
        positions[i].colonne = 1;
    }

    // Tant que l'utilisateur n'appuie pas sur Entrée, on vide le buffer pour éviter les problèmes de saisie
    while (getchar() != '\n') {
    }


    int joueur_actuel = 0;

    while (1) {
        screen_clear();
        screen_middle("===== GRILLE DU JOUEUR =====");

        printf("\nAu tour de: %s\n",
               parametres->joueurs[joueur_actuel].nom);
        printf("Symbole: %c\n",
               parametres->joueurs[joueur_actuel].symbole);

        Affiche_Grille(parametres->lignes, parametres->colonnes, positions, parametres->joueurs, nb_joueurs);
        
        printf("\nVoulez-vous aller à droite (d) ou en bas (b) ? ");
        fflush(stdout);
        char choix = getchar();
        vider_ligne();

        if (choix != 'd' && choix != 'D'
            && choix != 'b' && choix != 'B') {
            printf("Choix invalide. Veuillez choisir 'd' pour droite ou 'b' pour bas.\n");
            continue; // Recommence le tour du joueur actuel
        }

        int maximum = (choix == 'd' || choix == 'D') ? parametres->droite_max : parametres->bas_max;
        int deplacement = lire_deplacement(maximum);

        if (choix == 'd' || choix == 'D') {
            positions[joueur_actuel].colonne += deplacement;
        } else {
            positions[joueur_actuel].ligne += deplacement;
        }

        if (positions[joueur_actuel].ligne >= parametres->lignes && positions[joueur_actuel].colonne >= parametres->colonnes) {
            printf("\nFélicitations %s ! Vous avez atteint le puits et gagné la partie !\n", parametres->joueurs[joueur_actuel].nom);
            break;
        } else if (positions[joueur_actuel].ligne > parametres->lignes || positions[joueur_actuel].colonne > parametres->colonnes) {
            printf("\n%s a dépassé les limites de la grille et a perdu son tour.\n", parametres->joueurs[joueur_actuel].nom);
            if (choix == 'd' || choix == 'D') {
                positions[joueur_actuel].colonne -= deplacement;
            } else {
                positions[joueur_actuel].ligne -= deplacement;
            }
        }

        printf("Appuyez sur Entree pour continuer...");
        getchar();
        // Calcul: On passe au joueur suivant en utilisant l'opérateur modulo pour revenir au premier joueur après le dernier.
        // Exemple: Si il y a 3 joueurs (0, 1, 2) et que le joueur actuel est le joueur 2, alors (2 + 1) % 3 = 0, donc on revient au joueur 0.
        joueur_actuel = (joueur_actuel + 1) % nb_joueurs;
    }
}


void Game_Setup(int *Nb_Joueurs, T_Parametres *parametres)
{
    for (int i = 0; i < *Nb_Joueurs; i++) {
        printf("Nom du joueur %d : ", i + 1);
        if (scanf("%49s", parametres->joueurs[i].nom) != 1) {
            return;
        }

        printf("Symbole du joueur %d : ", i + 1);
        if (scanf(" %c", &parametres->joueurs[i].symbole) != 1) {
            return;
        }
    }
}

void Affiche_Grille(int L, int C, const T_Case positions[], const Joueur joueurs[], int nombre_joueurs)
{
    for (int ligne = 1; ligne <= L; ligne++) {

        for (int colonne = 1; colonne <= C; colonne++) {
            printf("+---");
        }
        printf("+\n");

        for (int colonne = 1; colonne <= C; colonne++) {
            char symbole = ' ';

            for (int joueur = 0; joueur < nombre_joueurs; joueur++) {

                // Si la position du joueur correspond à la case actuelle, on récupère son symbole
                if (ligne == positions[joueur].ligne
                    && colonne == positions[joueur].colonne) {
                    symbole = joueurs[joueur].symbole;
                    break;
                }
            }
            if (symbole == ' ' && ligne == L && colonne == C) {
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