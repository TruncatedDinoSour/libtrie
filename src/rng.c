#include "rng.h"

void rng_seed(RNGState *rng, const uint8_t *seed, const uint64_t seed_size) {
    uint64_t idx;

    rng->state = 0;

    for (idx = 0; idx < seed_size; ++idx)
        rng->state = (rng->state << 8) | seed[idx];

    rng->state |= 1;
}

uint32_t rng_next(RNGState *rng) {
    rng->state = (uint32_t)(rng->state * 2854097789 + 3968177761);
    return (uint32_t)(rng->state % 2147483648);
}
