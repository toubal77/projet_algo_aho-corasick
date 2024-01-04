#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h> 
#include "trie.h" 

#define BUFFSIZE_INIT 64  // Taille initiale du buffer pour la lecture des mots depuis un fichier

// Structure représentant un trie
struct _trie {
    int maxNode;
    int nextNode;
    int **transition;
    size_t *finite;
};#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h> 
#include "trie.h" 

#define BUFFSIZE_INIT 64  // Taille initiale du buffer pour la lecture des mots depuis un fichier

// Structure représentant un trie
struct _trie {
    int maxNode;
    int nextNode;
    int **transition;
    size_t *finite;
};

// Fonction pour créer un nouveau trie
Trie createTrie(int maxNode) {
    if (maxNode < 1) {
        return NULL;
    }

    struct _trie *trie = malloc(sizeof *trie);
    if (trie == NULL) {
        return NULL;
    }
    trie -> maxNode = maxNode;
    trie -> nextNode = 1;
    trie -> transition = NULL;
    trie -> finite = NULL;
    trie -> transition = malloc((size_t)maxNode  * sizeof (*trie -> transition));

    if (trie -> transition == NULL) {
        _removeTrie(&trie);
    }

    for (int **transition = trie -> transition;  transition < trie -> transition + maxNode; ++transition) {
        *transition = malloc(((int)UCHAR_MAX + 1) * sizeof **transition);
        if (*transition == NULL) {
            _removeTrie(&trie);
        }
        for (int *trans = *transition; trans <= *transition + UCHAR_MAX; ++trans) {
            *trans = -1;
        }
    }
    
    trie -> finite = malloc((size_t)maxNode * sizeof (*trie -> finite));
    if (trie -> finite == NULL) {
         _removeTrie(&trie);
    }
    for (size_t* final = trie -> finite; final < trie -> finite + maxNode; ++final) {
        *final = 0;
    }

    return trie;
}

void initTrie(Trie trie) {
    int *start = trie -> transition[0];
    for (int *transition = start; transition <= start + UCHAR_MAX; transition++) {
        if (*transition == -1) {
            *transition = 0;
        }
    }
}

// Fonction pour obtenir le prochain état dans le trie
int getTransition(Trie trie, int node, unsigned char letter) {
    return trie -> transition[node][letter];
}

// Fonction pour obtenir le nombre d'occurrences finales pour un état donné
size_t getFinitesNumber(Trie trie, int node) {
    return trie -> finite[node];
}