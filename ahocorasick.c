#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "ahocorasick.h"

// Structure représentant une file utilisée dans l'algorithme d'Aho-Corasick
struct _queue {
    size_t capacite;  // Capacité maximale de la file
    size_t tete;      // Indice de la tête de la file
    size_t tail;      // Indice de la queue de la file
    int* queue;       // Tableau contenant les éléments de la file
};

// 

// Structure représentant un automate d'Aho-Corasick
struct _ac {
    Trie trie;  // Trie associé à l'automate d'Aho-Corasick
    int supp[];  // Tableau de suppléance pour les transitions
};

AC createAC(Trie trie);
size_t getOccurences(AC ac, FILE *file) ;
Queue createQueue(size_t capacite);
void completer(AC ac);
bool isEmptyQueue(Queue q);
void pushQueue(Queue q, int elem) ;
int popQueue(Queue q);
void ACremove(AC* ac);
void removeQueue(Queue *q);
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


// Fonction pour créer une nouvelle file avec une capacité donnée
Queue createQueue(size_t capacite) {
    struct _queue *q = malloc(sizeof(struct _queue));
    if (q == NULL) {
        return NULL;
    }

    q->capacite = capacite + 1;
    q->tete = 0;
    q->tail = 0;
    q->queue = malloc(q->capacite * sizeof(int));
    if (q->queue == NULL) {
        removeQueue(&q);
    }

    return q;
}

// Fonction pour effectuer la phase de complétion de l'automate d'Aho-Corasick
void completer(AC ac) {
    // Création d'une file pour l'algorithme de complétion
    Queue file = createQueue(getTailleTrie(ac->trie));
    if (file == NULL) {
        ACremove(&ac);
        return;
    }

    // Initialisation de la file avec les transitions de l'état initial
    for (size_t a = 0; a <= UCHAR_MAX; a++) {
        int transition = getTransition(ac->trie, 0, (unsigned char)a);
        if (transition != 0) {
            pushQueue(file, transition);
            ac->supp[transition] = 0;
        }
    }

    // Algorithme de complétion
    while (!isEmptyQueue(file)) {
        int node = popQueue(file);

        for (size_t a = 0; a <= UCHAR_MAX; a++) {
            int p = getTransition(ac->trie, node, (unsigned char)a);
            if (p != -1) {
                pushQueue(file, p);
                int s = ac->supp[node];
                while (getTransition(ac->trie, s, (unsigned char)a) == -1) {
                    s = ac->supp[s];
                }
                ac->supp[p] = getTransition(ac->trie, s, (unsigned char)a);
                addFinite(ac->trie, p, getFinitesNumber(ac->trie, ac->supp[p]));
            }
        }
    }

    // Libération de la mémoire de la file
    removeQueue(&file);
}

// Fonction pour vérifier si la file est vide
bool isEmptyQueue(Queue q) {
    return q->tete == q->tail;
}

// Fonction pour ajouter un élément à la queue de la file
void pushQueue(Queue q, int elem) {
    if ((q->tail + 1) % q->capacite != q->tete) {
        q->queue[q->tail] = elem;
        q->tail = (q->tail + 1) % q->capacite;
    }
}

// Fonction pour retirer un élément de la tête de la file
int popQueue(Queue q) {
    int ret = q->queue[q->tete];
    if (q->tete != q->tail) {
        q->tete = (q->tete + 1) % q->capacite;
    }
    return ret;
}

// Fonction pour libérer la mémoire associée à un automate d'Aho-Corasick
void ACremove(AC* ac) {
    free(*ac);
    *ac = NULL;
}

void removeQueue(Queue *q) {
    if ((*q)->queue != NULL) {
        free((*q)->queue);
    }
    free(*q);
    *q = NULL;
}
