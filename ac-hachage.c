#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "trie.h"

// Facteur de charge utilisé pour déterminer la taille initiale du tableau de transitions
#define LOAD_FACTOR 0.75

// Taille initiale du buffer pour la lecture des mots depuis un fichier
#define BUFFSIZE_INIT 64

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

void removeTrie(Trie *trie);
static void _removeTrie(struct _trie** trie) ;
void initTrie(Trie trie);
Trie createTrie(int maxNode);
void addFinite(Trie trie, int node, size_t nbOcc);
size_t getFinitesNumber(Trie trie, int node);
int motFichier(FILE *file, char **buff, size_t *buffSize);
int getWordFromFile(Trie trie, FILE *file) ;
void insertWord(Trie trie, unsigned char *mot) ;
static int AjouterTransition(struct _trie *trie, int startNode,
        unsigned char letter, int targetNode);
static inline size_t hashFunction(int node, unsigned char letter) ;
static int ProchainEtat(struct _trie *trie, int node, unsigned char letter);
size_t getTaille(int maxNode);
int getTransition(Trie trie, int node, unsigned char letter);
size_t getTailleTrie(Trie trie);
// Fonction pour libérer la mémoire associée à un trie (interface publique)
void removeTrie(Trie *trie) {
    _removeTrie(trie);
}

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

// Fonction pour initialiser le trie en ajoutant les transitions manquantes pour chaque caractère possible
void initTrie(Trie trie) {
    for (size_t c = 0; c <= UCHAR_MAX; c++) {
        // Ajout de la transition si elle n'existe pas déjà
        if (ProchainEtat(trie, 0, (unsigned char) c) == -1) {
            AjouterTransition(trie, 0, (unsigned char) c, 0);
        }
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

// Fonction pour ajouter des occurrences finales à un état donné
void addFinite(Trie trie, int node, size_t nbOcc) {
    trie -> finite[node] += nbOcc;
}

// Fonction pour obtenir le nombre d'occurrences finales pour un état donné
size_t getFinitesNumber(Trie trie, int node) {
    return trie -> finite[node];
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
            return *(*buff + len) == '\0' ? 0 : -1;
        }

        len = strlen(*buff);
        end = (*buff)[len - 1];
    }

    // Remplacement du caractère de fin de ligne par '\0'
    (*buff)[len - 1] = '\0';

    return 0;
}


// Fonction pour ajouter des mots depuis un fichier au trie
int getWordFromFile(Trie trie, FILE *file) {
    size_t buffSize = BUFFSIZE_INIT;
    char *buff = malloc(BUFFSIZE_INIT);
    if (buff == NULL) {
        return -1;
    }

    // Lecture et ajout de chaque mot du fichier au trie
    while (motFichier(file, &buff, &buffSize) != -1) {
        insertWord(trie, (unsigned char *) buff);
    }

    free(buff);
    return 0;
}

// Fonction pour ajouter un mot au trie
void insertWord(Trie trie, unsigned char *mot) {
    struct _trie *t = trie;
    int old_state = 0;
    int curr_state = 0;
    unsigned char *word = mot;

    // Parcours du mot et mise à jour du trie
    while (*word != '\0' && curr_state != -1) {
        old_state = curr_state;
        curr_state = ProchainEtat(t, curr_state, *word);
        ++word;
    }

    // Ajout des transitions manquantes pour le reste du mot
    if (curr_state == -1) {
        curr_state = old_state;
        --word;
        // Vérification du nombre maximal d'états dans le trie
        if (strlen((char*)word) > (size_t)(t -> maxNode - t -> nextNode)) {
            printf("Error: maximum number of nodes exceeded\n");
            return;
        }
        // Ajout des transitions manquantes pour le reste du mot
        while (*word != '\0') {
            AjouterTransition(t, curr_state, *word, t -> nextNode);
            curr_state = t -> nextNode;
            t -> nextNode++;
            ++word;
        }
    }

    // Incrémentation du nombre d'occurrences finales pour le dernier état
    t -> finite[curr_state]++;
}


// Fonction utilitaire pour ajouter une transition au trie
static int AjouterTransition(struct _trie *trie, int startNode,
        unsigned char letter, int targetNode) {
    // Allocation de mémoire pour la nouvelle transition
    struct _list *nouvelleTransition = malloc(sizeof *nouvelleTransition);
    if (nouvelleTransition == NULL) {
        return -1;
    }
    // Initialisation des champs de la nouvelle transition
    nouvelleTransition -> startNode = startNode;
    nouvelleTransition -> targetNode = targetNode;
    nouvelleTransition -> letter = letter;

    // Calcul de l'index dans le tableau des transitions
    size_t tailleTransition = getTaille(trie -> maxNode);
    size_t index = hashFunction(startNode, letter) % tailleTransition;

    // Ajout de la nouvelle transition en tête de liste
    nouvelleTransition -> next = trie -> transition[index];
    trie -> transition[index] = nouvelleTransition;
    return 0;
}


// Fonction utilitaire pour calculer la valeur de hachage d'une transition
static inline size_t hashFunction(int node, unsigned char letter) {
    return (size_t)node * ((size_t)UCHAR_MAX + 1) + (size_t)letter;
}

// Fonction utilitaire pour obtenir le prochain état dans le trie
static int ProchainEtat(struct _trie *trie, int node, unsigned char letter) {
    size_t index = hashFunction(node, letter) % getTaille(trie -> maxNode);
    int next = -1;
    struct _list *transition = trie -> transition[index];
    while (transition != NULL && next == -1) {
        if (transition -> startNode == node && transition -> letter == letter) {
            next = transition -> targetNode;
        }
        transition = transition -> next;
    }

    return next;
}

// Fonction qui calcule la taille du tableau 'finite'
size_t getTaille(int maxNode) {
    return (size_t)(((maxNode) - 1) / LOAD_FACTOR + 1);
}

// Fonction pour obtenir le prochain état dans le trie
int getTransition(Trie trie, int node, unsigned char letter) {
    return ProchainEtat(trie, node, letter);
}


// Fonction pour obtenir la taille du trie (nombre d'états utilisés)
size_t getTailleTrie(Trie trie) {
    return (size_t) trie -> nextNode;
}