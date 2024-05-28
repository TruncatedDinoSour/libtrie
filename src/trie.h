#ifndef _TRIE_H
#define _TRIE_H
#include <stdio.h>
#include <stdint.h>

typedef struct Trie {
    uint8_t c;
    uint8_t n;
    struct Trie **children;
} Trie;

/* Node operations */

Trie *trie_create_node(const uint8_t c);
Trie *trie_find_child(const Trie *node, const uint8_t c);
void trie_insert_character(Trie *node, const uint8_t c);
void trie_insert_sentence(Trie *root, const uint8_t *sentence);

/* FS operations */

void trie_save_file(FILE *file, const Trie *node);
Trie *trie_load_file(FILE *file);

/* Trie operations */
void trie_free(Trie *node);
#endif /* _TRIE_H */
