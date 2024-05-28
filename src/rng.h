#ifndef _TRIE_RNG_H
#define _TRIE_RNG_H
#include <stdint.h>

typedef struct TrieRNG {
    uint32_t state;
} TrieRNG;

#define RNG_MAX UINT32_MAX

#ifndef ITER_MAGIC
#define ITER_MAGIC (uint32_t)2170632407 /* Random prime number */
#endif                                  /* ITER_MAGIC */

#define rng_iter(state, count, idx) \
    (state) ^= ((count) - (idx) + ITER_MAGIC + ((idx) % 2 == 0))

void rng_seed(TrieRNG *rng, const uint8_t *seed, const uint64_t seed_size);
uint32_t rng_next(TrieRNG *rng);
#endif /* _TRIE_RNG_H */
