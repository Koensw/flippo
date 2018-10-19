#ifndef RAND_H
#define RAND_H

#include <cstdint>
#include <random>

class Rand {
public:
    Rand();

    void seed();

    // *Really* minimal PCG32 code / (c) 2014 M.E. O'Neill / pcg-random.org
    // Licensed under Apache License 2.0 (NO WARRANTY, etc. see website)
    inline uint32_t operator()() {
        uint64_t oldstate = state;
        state = oldstate * 6364136223846793005ULL + (inc | 1);
        uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
        uint32_t rot = oldstate >> 59u;
        return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
    }

private:
    uint64_t state;
    uint64_t inc;
};

extern Rand rng;

#endif
