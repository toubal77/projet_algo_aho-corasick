#ifndef TRIE_H
#define TRIE_H
#include <stdio.h>
#include <stdbool.h>
typedef struct _trie *Trie;

Trie createTrie(int maxNode);

void removeTrie(Trie *trie);

int getWordFromFile(Trie trie, FILE *file);

int getTransition(Trie trie, int node, unsigned char letter);

void initTrie(Trie trie);

size_t getFinitesNumber(Trie trie, int node);

size_t getTailleTrie(Trie trie);

void addFinite(Trie trie, int node, size_t nbOcc);

#endif
