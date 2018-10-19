#include "rand.h"

#include <chrono>

Rand rng;
Rand::Rand() {
    state = 0U;
    inc = (54u << 1u) | 1u;
}

void Rand::seed() {
    typedef std::chrono::high_resolution_clock highres_clock;
    highres_clock::time_point time = highres_clock::now();
    state = 0U;
    inc = (54u << 1u) | 1u;
    operator()();
    state += time.time_since_epoch().count();
    operator()();
    return;
}
