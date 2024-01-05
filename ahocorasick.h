#ifndef AHOCORASICK_H
#define AHOCORASICK_H
#include <stdio.h>
#include "trie.h"

typedef struct _ac* AC;

AC createAC(Trie trie);

size_t getOccurences(AC ac, FILE *file);

#endif