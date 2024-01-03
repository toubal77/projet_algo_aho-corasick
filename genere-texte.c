#include <stdlib.h>
#include <stdio.h>
#include "genere-texte.h"

void generateurTexte(int n, int alphabetSize);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Nombre d'arguments incorrect\n");
        return 1;
    }

    int n = atoi(argv[1]);
    int alphabetSize = atoi(argv[2]);

    generateurTexte(n, alphabetSize);

    return 0;
}

void generateurTexte(int n, int alphabetSize) {
    if (n <= 0 || alphabetSize <= 0) {
        fprintf(stderr, "Longueur de texte ou taille d'alphabet non valide.\n");
        return;
    }

    srand((unsigned int)time(NULL));

    for (int i = 0; i < n; i++) {
        char randomChar = 'a' + (char)(rand() % alphabetSize);
        putchar(randomChar);
    }
}