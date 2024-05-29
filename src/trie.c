#include "trie.h"
#include "null.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

const uint16_t trie_version = 1;

Trie *trie_create_node(const uint8_t c) {
    Trie *node;

    node = malloc(sizeof(Trie));

    if (!node)
        return NULL;

    node->c        = c;
    node->children = NULL;
    node->n        = 0;

    return node;
}

Trie *trie_find_child(const Trie *node, const uint8_t c) {
    uint8_t idx;

    for (idx = 0; idx < node->n; ++idx)
        if (node->children[idx]->c == c)
            return node->children[idx];

    return NULL;
}

Trie *trie_insert_character(Trie *node, const uint8_t c) {
    Trie *child;

    if (!node)
        return NULL;

    child = trie_find_child(node, c);

    if (!child) {
        child = trie_create_node(c);

        node->children =
            realloc(node->children, (node->n + 1) * sizeof(Trie *));

        node->children[node->n++] = child;
    }

    return child;
}

Trie *trie_insert_sentence(Trie *root, const uint8_t *sentence) {
    uint8_t c;
    Trie *current;

    if (!sentence)
        return NULL;

    current = root;

    while (*sentence) {
        c       = *sentence;
        current = trie_insert_character(current, c);
        ++sentence;
    }

    return trie_insert_character(current, '\0');
}

void trie_save_file(FILE *file, const Trie *node) {
    uint8_t idx;

    if (!node)
        return;

    fwrite(&(node->c), sizeof(uint8_t), 1, file);
    fwrite(&(node->n), sizeof(uint8_t), 1, file);

    for (idx = 0; idx < node->n; ++idx)
        trie_save_file(file, node->children[idx]);
}

Trie *trie_load_file(FILE *file) {
    uint8_t c;
    uint8_t n;
    uint8_t idx;

    Trie *node;

    if (fread(&c, sizeof(uint8_t), 1, file) != 1)
        return NULL;

    if (fread(&n, sizeof(uint8_t), 1, file) != 1)
        return NULL;

    node = malloc(sizeof(Trie));

    if (!node)
        return NULL;

    node->c        = c;
    node->n        = n;
    node->children = malloc(n * sizeof(Trie *));

    for (idx = 0; idx < n; ++idx)
        node->children[idx] = trie_load_file(file);

    return node;
}

void trie_free(Trie *node) {
    uint8_t idx;

    if (!node)
        return;

    for (idx = 0; idx < node->n; ++idx)
        trie_free(node->children[idx]);

    free(node->children);
    free(node);
}
