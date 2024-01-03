#include <stdlib.h>
#include <stdio.h>
#include "genere-mots.h"

void genererMot(int nbMot, int longueurMin, int longueurMax, int alphabetSize);

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <nombre_de_mots> <longueur_minimale> <longueur_maximale> <taille_alphabet>\n", argv[0]);
        return 1;
    }

    int nbMot = atoi(argv[1]);
    int longueurMin = atoi(argv[2]);
    int longueurMax = atoi(argv[3]);
    int alphabetSize = atoi(argv[4]);

    genererMot(nbMot, longueurMin, longueurMax, alphabetSize);

    return 0;
}

void genererMot(int nbMot, int longueurMin, int longueurMax, int alphabetSize) {
    if (nbMot <= 0 || longueurMin <= 0 || longueurMax < longueurMin || alphabetSize <= 0) {
        printf("ParamÃ¨tres non valides.\n");
        return;
    }

    srand((unsigned int)time(NULL)); 

    for (int i = 0; i < nbMot; i++) {
        int longueurMot = longueurMin + (rand() % (longueurMax - longueurMin + 1));

        for (int j = 0; j < longueurMot; j++) {
            char randomChar = 'a' + (char)(rand() % alphabetSize);
            putchar(randomChar);
        }

        putchar('\n');
    }
}