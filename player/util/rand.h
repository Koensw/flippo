#ifndef RAND_H
#define RAND_H

#include <cstdint>
#include <random>

class Rand {
public:
    Rand();

    void seed();

    inline uint32_t operator()() { return generator(); }

private:
    std::mt19937 generator;
};

extern Rand rng;

#endif
