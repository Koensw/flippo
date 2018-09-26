#ifndef STANDARD_MCTS_STRATEGY_H
#define STANDARD_MCTS_STRATEGY_H

#include "../game/strategy.h"
#include "../mcts/mcts.hpp"

#include <memory>

class StandardMCTSStrategy : public Strategy {
    void start(Player::Color);
    void update(Index idx);

private:
    Index play() override;
    int simulations_;

    std::unique_ptr<MCTSBase> mcts_;
};

#endif
