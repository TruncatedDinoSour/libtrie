#ifndef _TRIE_RNG_H
#define _TRIE_RNG_H
#include <stdint.h>

typedef struct {
    uint32_t state;
} RNGState;

void rng_seed(RNGState *rng, const uint8_t *seed, const uint64_t seed_size);
uint32_t rng_next(RNGState *rng);
#endif /* _TRIE_RNG_H */
