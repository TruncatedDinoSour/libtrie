#ifndef _TRIE_GEN_H
#define _TRIE_GEN_H

#include "rng.h"
#include "trie.h"

#include <stdint.h>

uint8_t *gen_trie_random(const Trie *node, RNGState *rng, uint64_t min_length, uint64_t *size);
#endif /* _TRIE_GEN_H */
