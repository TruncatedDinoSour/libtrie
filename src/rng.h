#ifndef _TRIE_RNG_H
#define _TRIE_RNG_H
#include <stdint.h>

typedef struct TrieRNG {
    uint32_t state;
} TrieRNG;

#define RNG_MAGIC  (uint32_t)((1 << (32 - 1)) | 1)
#define ITER_MAGIC (uint32_t)4149861197 /* Magic prime number */

#define rng_iter(state, count, idx) \
    (state) += (uint32_t)(((count) - (idx) + ITER_MAGIC) | RNG_MAGIC);

void rng_seed(TrieRNG *rng, const uint8_t *seed, const uint64_t seed_size);
uint32_t rng_next(TrieRNG *rng);
#endif /* _TRIE_RNG_H */
