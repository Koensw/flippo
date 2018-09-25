#include "rand.h"

#include <chrono>

Rand rng;

Rand::Rand() {
    generator.seed(0);
}

void Rand::seed() {
    typedef std::chrono::high_resolution_clock highres_clock;
    highres_clock::time_point time = highres_clock::now();
    generator.seed(time.time_since_epoch().count());
    return;
}
