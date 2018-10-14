#ifndef STRATEGY_H
#define STRATEGY_H

#include "board.h"
#include "player.h"

#include <chrono>

class Strategy {
public:
    Strategy();

    virtual void start(Player::Color c);
    virtual void update(Index idx);
    virtual Index step();

    Board getBoard() const;

    void pause();
    double getElapsed() const;
    void resume();

protected:
    virtual Index play() = 0;

    Board brd_;
    Player pl_;

    bool pause_;
    std::chrono::high_resolution_clock::time_point pause_start_;
    std::chrono::high_resolution_clock::time_point start_;
    double extra_;
};

#endif
