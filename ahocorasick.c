#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "ahocorasick.h"


// Structure représentant un automate d'Aho-Corasick
struct _ac {
    Trie trie;  // Trie associé à l'automate d'Aho-Corasick
    int supp[];  // Tableau de suppléance pour les transitions
};

// Fonction pour créer un nouvel automate d'Aho-Corasick à partir d'un trie donné
AC createAC(Trie trie) {
    struct _ac *ac = malloc((sizeof *ac) + getTailleTrie(trie) * (sizeof (size_t)));
    if (ac == NULL) {
        return NULL;
    }

    // Initialisation du trie associé à l'automate
    initTrie(trie);
    ac->trie = trie;

    // Complétion de l'automate
    completer(ac);

    return ac;
}

// Fonction pour obtenir le nombre d'occurrences des motifs dans un fichier
size_t getOccurences(AC ac, FILE *file) {
    size_t nbOcc = 0;
    int node = 0;

    // Parcours du fichier caractère par caractère
    for (int c = fgetc(file); c != EOF; c = fgetc(file)) {
        // Mise à jour de l'état du trie en suivant les transitions
        while (getTransition(ac->trie, node, (unsigned char)c) == -1) {
            node = ac->supp[node];
        }
        node = getTransition(ac->trie, node, (unsigned char)c);

        // Incrémentation du nombre d'occurrences finales
        nbOcc += getFinitesNumber(ac->trie, node);
    }

    return nbOcc;
}