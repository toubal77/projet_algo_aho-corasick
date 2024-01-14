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
    if (file_mots == NULL) {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Ouverture du fichier texte dans lequel rechercher les mots
    FILE *file_texte = fopen(argv[2], "r");
    if (file_texte == NULL) {
        fclose(file_mots);
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier %s\n", argv[2]);
        return EXIT_FAILURE;
    }

    // Calcul de la taille du fichier mots.txt
    fseek(file_mots, 0, SEEK_END);
    int size_file_mots = (int) ftell(file_mots);
    rewind(file_mots);

    // Création d'un trie pour stocker les mots à rechercher
    Trie trie = createTrie(size_file_mots);
    if (trie == NULL) {
        fclose(file_mots);
        fprintf(stderr, "Erreur : impossible de créer le trie\n");
        return EXIT_FAILURE;
    }

    // Insertion des mots dans le trie
    if (getWordFromFile(trie, file_mots) == -1) {
        removeTrie(&trie);
        fclose(file_mots);
        fprintf(stderr, "Erreur : échec de l'insertion des mots dans la matrice de transitions\n");
        return EXIT_FAILURE;
    }

    fclose(file_mots);

    // Création de l'automate d'Aho-Corasick à partir du trie
    AC ac = createAC(trie);
    if (ac == NULL) {
        removeTrie(&trie);
        fprintf(stderr, "Erreur : impossible de créer la structure d'Aho-Corasick\n");
        return EXIT_FAILURE;
    }

    // Recherche des occurrences dans le fichier texte
    printf("Nombre d'occurrences : %lu\n", getOccurences(ac, file_texte));

    fclose(file_texte);
    
    // Libération de la mémoire allouée pour le trie et l'automate d'Aho-Corasick
    removeTrie(&trie);
    ACremove(&ac);

    return EXIT_SUCCESS;
}
