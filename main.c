#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "trie.h"
#include "ahocorasick.h"

// Fonction principale du programme
int main(int argc, char **argv) {
    // Vérification du nombre d'arguments en ligne de commande
    if (argc != 3) {
        fprintf(stderr,"Nombre d'arguments invalides \n");
        return EXIT_FAILURE;
    }
    // Ouverture du fichier contenant les mots à rechercher
    FILE *file_mots = fopen(argv[1], "r");

    // Ouverture du fichier texte dans lequel rechercher les mots
    FILE *file_texte = fopen(argv[2], "r");

    // Calcul de la taille du fichier mots.txt
    fseek(file_mots, 0, SEEK_END);
    int size_file_mots = (int) ftell(file_mots);
    rewind(file_mots);

    // Création d'un trie pour stocker les mots à rechercher de la meme taille
    Trie trie = createTrie(size_file_mots);

    // Création de l'automate d'Aho-Corasick à partir du trie
    AC ac = createAC(trie);

    // Recherche des occurrences dans le fichier texte
    printf("Nombre d'occurrences : %lu\n", getOccurences(ac, file_texte));

    fclose(file_texte);
    fclose(file_mots);


    return EXIT_SUCCESS;
}