#ifndef AHOCORASICK_H
#define AHOCORASICK_H
#include <stdio.h>
#include "trie.h"

typedef struct _queue* Queue;

typedef struct _ac* AC;

AC createAC(Trie trie);

void ACremove(AC* ac);

size_t getOccurences(AC ac, FILE *file);

#endif