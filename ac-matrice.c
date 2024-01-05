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
};

// Fonction utilitaire pour libérer la mémoire associée à un trie
static void _removeTrie(struct _trie** trie) {
    if (*trie != NULL) {
        for (int **transition = (*trie) -> transition; 
                transition < (*trie) -> transition + (*trie) -> maxNode;
                ++transition) {
            free(*transition);
        }
        free((*trie) -> transition);
        free((*trie) -> finite);
        free(*trie);
        *trie = NULL;
    }
}

// Fonction pour libérer la mémoire associée à un trie (interface publique)
void removeTrie(Trie *trie) {
    _removeTrie(trie);
}

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

// Fonction pour obtenir la taille du trie (nombre d'états utilisés)
size_t getTailleTrie(Trie trie) {
    return (size_t) trie -> nextNode;
}

// Fonction pour lire un mot depuis un fichier et mettre à jour le buffer
int motFichier(FILE *file, char **buff, size_t *buffSize) {
    char *mot = fgets(*buff, (int) *buffSize, file);
    if (mot == NULL) {
        return -1;
    }
    size_t len = strlen(*buff);
    char end = (*buff)[len - 1];
    // Élargissement du buffer si le mot n'est pas complètement lu
    while (end != '\n') {
        *buffSize *= 2;
        mot = realloc(*buff, *buffSize);
        if (mot == NULL) {
            return -1;
        }

        mot = fgets(*buff + len, (int) *buffSize / 2, file);
        if (mot == NULL) {
            return *(*buff + len) == '\0' ? -1 : 0;
        }

        len = strlen(*buff);
        end = (*buff)[len - 1];
    }

    // Remplacement du caractère de fin de ligne par '\0'
    (*buff) [len - 1] = '\0';

    return 0;
}

// Fonction pour ajouter un mot au trie
void insertWord(Trie trie, unsigned char *mot) {
    struct _trie *t = trie;
    int old_state = 0;
    int curr_state = 0;
    unsigned char *word = mot;
    
    while (*word != '\0' && curr_state != -1) {
        old_state = curr_state;
        curr_state = t -> transition[curr_state][*word];
        ++word;
    }
    if (curr_state == -1) {
        curr_state = old_state;
        --word;
        if (strlen((char*)word) > (size_t)(t -> maxNode - t -> nextNode)) {
            printf("Error: nodes number exceeded\n");
            return;
        }
        while (*word != '\0') {
            t -> transition[curr_state][*word] = t -> nextNode;
            curr_state = t -> nextNode;
            t ->nextNode++;
            ++word;
        }
    }
    
    t -> finite[curr_state]++;
}

// Fonction pour ajouter des mots depuis un fichier au trie
int getWordFromFile(Trie trie, FILE *file) {
    size_t buffSize = BUFFSIZE_INIT;
    char *buff = malloc(BUFFSIZE_INIT);
    if (buff == NULL) {
        return -1;
    }
    
    while (motFichier(file, &buff, &buffSize) != -1) {
        insertWord(trie, (unsigned char *) buff);
    }

    free(buff);
    return 0;
}

// Fonction pour obtenir le prochain état dans le trie
int getTransition(Trie trie, int node, unsigned char letter) {
    return trie -> transition[node][letter];
}

// Fonction pour obtenir le nombre d'occurrences finales pour un état donné
size_t getFinitesNumber(Trie trie, int node) {
    return trie -> finite[node];
}

// Fonction pour ajouter des occurrences finales à un état donné
void addFinite(Trie trie, int node, size_t nbOcc) {
    trie -> finite[node] += nbOcc;
}

// Fonction pour initialiser le trie en ajoutant les transitions manquantes pour chaque caractère possible
void initTrie(Trie trie) {
    int *start = trie -> transition[0];
    for (int *transition = start; transition <= start + UCHAR_MAX; transition++) {
        if (*transition == -1) {
            *transition = 0;
        }
    }
}
