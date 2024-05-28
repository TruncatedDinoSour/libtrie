#include "gen.h"
#include "rng.h"
#include "null.h"
#include "trie.h"

#include <stdlib.h>
#include <stdint.h>

uint8_t *gen_trie_random(const Trie *node,
                         RNGState *rng,
                         uint64_t min_length,
                         uint64_t *size) {
    static uint8_t *buffer      = NULL;
    static uint64_t buffer_size = 0;
    static uint64_t length      = 0;

    uint8_t idx;
    uint8_t *final_buffer;

    const Trie *current;

    if (buffer == NULL) {
        buffer_size = 256;
        buffer      = malloc(buffer_size);

        if (!buffer) {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }
    }

    current = node;

    while (current && current->n > 0) {
        idx     = (uint8_t)rng_next(rng) % current->n;
        current = current->children[idx];

        if (current->c == '\0')
            break;

        if (length >= buffer_size - 1) {
            buffer_size *= 2;
            buffer = realloc(buffer, buffer_size);

            if (!buffer) {
                perror("Failed to reallocate memory");
                exit(EXIT_FAILURE);
            }
        }

        buffer[length++] = current->c;
    }

    if (length < min_length) {
        if (length >= buffer_size - 1) {
            buffer_size *= 2;
            buffer = realloc(buffer, buffer_size);

            if (!buffer) {
                perror("Failed to reallocate memory");
                exit(EXIT_FAILURE);
            }
        }

        buffer[length++] = ' ';
        return gen_trie_random(node, rng, min_length, size);
    } else {
        buffer[length] = '\0';
        *size          = length;
        final_buffer   = buffer;

        buffer      = NULL;
        buffer_size = 0;
        length      = 0;

        return final_buffer;
    }

    return NULL;
}
