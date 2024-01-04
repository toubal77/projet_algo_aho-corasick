
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "trie.h"

// Structure représentant un trie
struct _list {
    int startNode;
    int targetNode;
    unsigned char letter;     
    struct _list *next;      
};

typedef struct _list *List;

// Structure représentant un trie
struct _trie {
    int maxNode;
    int nextNode;
    List *transition;
    size_t *finite;
};

// Fonction utilitaire pour libérer la mémoire alloué par un trie
static void _removeTrie(struct _trie** trie) {
    // Libération de la mémoire pour chaque transition
    if (*trie != NULL) {
        size_t tailleT = getTaille((*trie) -> maxNode);
        for (List *transition = (*trie) -> transition;
                transition < (*trie) -> transition + tailleT; ++transition) {
            while (*transition != NULL) {
                struct _list *tmp = *transition;
                *transition = (*transition) -> next;
                free(tmp);
            }
        }
        // Libération de la mémoire pour les tableaux et la structure principale
        free((*trie) -> transition);
        free((*trie) -> finite);
        free(*trie);
        *trie = NULL;
    }
}


// Fonction pour créer un nouveau trie
Trie createTrie(int maxNode) {
    if (maxNode < 1) {
        return NULL;
    }
    // Allocation de mémoire pour la structure principale du trie
    struct _trie *trie = malloc(sizeof *trie);
    if (trie == NULL) {
        return NULL;
    }
    // Initialisation des champs de la structure du trie
    trie -> finite = NULL;
    trie -> transition = NULL;
    trie -> maxNode = maxNode;
    trie -> nextNode = 1;

    // Allocation de mémoire pour le tableau des transitions
    size_t tailleTransition = getTaille(maxNode);
    trie -> transition = malloc(tailleTransition * sizeof *trie -> transition);
    if (trie -> transition == NULL) {
         _removeTrie(&trie);
    }
    // Initialisation des listes de transitions
    for (List *transition = trie -> transition;
            transition < trie -> transition + tailleTransition; ++transition) {
        *transition = NULL;
    }

    // Allocation de mémoire pour le tableau des occurrences finales
    trie -> finite = malloc((size_t)maxNode * sizeof (*trie -> finite));
    if (trie -> finite == NULL) {
         _removeTrie(&trie);
    }
    // Initialisation des occurrences finales
    for (size_t* final = trie -> finite; final < trie -> finite + maxNode; ++final) {
        *final = 0;
    }

    return trie;
}

// Fonction pour obtenir la taille du trie (nombre d'états utilisés)
size_t getTailleTrie(Trie trie) {
    return (size_t) trie -> nextNode;
}