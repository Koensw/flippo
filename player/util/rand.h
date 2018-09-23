#ifndef RAND_H
#define RAND_H

#include <cstdint>
#include <random>

class Rand {
public:
    void seed();
    uint32_t operator()();

private:
    std::mt19937 generator;
};

extern Rand rng;

#endif
