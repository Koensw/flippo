#ifndef STRATEGY_H
#define STRATEGY_H

#include "player.h"
#include "board.h"

class Strategy{
public:
    virtual void start(Player::Color c);
    virtual void update(Index idx);
    virtual Index step();
    
    Board getBoard() const;
    
    virtual Index play() = 0;
protected:
    Board brd_;
};

#endif
