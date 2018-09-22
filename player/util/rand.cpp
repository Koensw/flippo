#include "rand.h"

#include <chrono>

Rand rng;

void Rand::seed(){
    typedef std::chrono::high_resolution_clock highres_clock;
    highres_clock::time_point time = highres_clock::now();
    generator.seed(time.time_since_epoch().count());
    return;
}

uint32_t Rand::operator()(){
    return generator();
}
