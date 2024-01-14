#ifndef AHOCORASICK_H
#define AHOCORASICK_H
#include <stdio.h>
#include "trie.h"

typedef struct _queue* Queue;

typedef struct _ac* AC;

AC createAC(Trie trie);
size_t getOccurences(AC ac, FILE *file) ;
Queue createQueue(size_t capacite);
void completer(AC ac);
bool isEmptyQueue(Queue q);
void pushQueue(Queue q, int elem) ;
int popQueue(Queue q);
void ACremove(AC* ac);
void removeQueue(Queue *q);

#endif