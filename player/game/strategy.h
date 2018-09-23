#ifndef STRATEGY_H
#define STRATEGY_H

#include "board.h"
#include "player.h"

class Strategy {
public:
    virtual void start(Player::Color c);
    virtual void update(Index idx);
    virtual Index step();

    Board getBoard() const;

protected:
    virtual Index play() = 0;

    Board brd_;
};

#endif
