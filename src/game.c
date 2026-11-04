
#include <stdio.h>
#include <string.h>
// Bibliothèque pour obtenir la taille du terminal
#include <sys/ioctl.h>

// Bibliothèque pour obtenir la taille du terminal
#include <unistd.h>
#include "game.h"
#include "menu.h"

int main(void)
{
    screen_clear();
    int rows, cols;
    int size = get_terminal_size(&rows, &cols);
    printf("Taille du terminal : %d lignes, %d colonnes\n", rows, cols);


}

