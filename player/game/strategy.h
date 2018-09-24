#ifndef STRATEGY_H
#define STRATEGY_H

#include "board.h"
#include "player.h"

#include <chrono>

class Strategy {
public:
    virtual void start(Player::Color c);
    virtual void update(Index idx);
    virtual Index step();

    Board getBoard() const;

    double getElapsed() const;

protected:
    virtual Index play() = 0;

    Board brd_;
    std::chrono::high_resolution_clock::time_point start_;
};

#endif
