#include "rng.h"

void rng_seed(TrieRNG *rng, const uint8_t *seed, const uint64_t seed_size) {
    uint64_t idx;

    rng->state = 0x4D595DF4;

    for (idx = 0; idx < seed_size; ++idx) {
        rng->state ^= (uint32_t)(seed[idx] << ((idx % 4) * 8));
        rng->state ^= (rng->state << 13);
        rng->state ^= (rng->state >> 17);
        rng->state ^= (rng->state << 5);
    }

    if (rng->state == 0)
        rng->state = 0x4D595DF4;
}

uint32_t rng_next(TrieRNG *rng) {
    uint32_t x = rng->state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    rng->state = x;
    return x;
}
