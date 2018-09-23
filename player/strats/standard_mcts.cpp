#include "standard_mcts.h"

#include "../mcts/mcts.hpp"
#include "../util/rand.h"

#include <climits>
#include <cmath>

void StandardMCTSStrategy::start(Player::Color c) {
    Strategy::start(c);

    auto selector = [](Node* p, Node* c) {
        auto scr = c->w / c->n + 1.4 * std::sqrt(std::log(p->n) / c->n);
        scr += 1e-9 * (rng() % 1000);
        return scr;
    };
    auto initializer = [](Node* n) { n->v = 0; };
    auto mover = [](const Board& brd, Player pl) {
        auto mvs = brd.getMoves(pl);
        return mvs[rng() % mvs.size()];
    };
    auto scorer = [](const Board& brd, Player pl) {
        auto scr = (brd.count(pl) - 2.0) / 60.0;
        return scr;
    };

    mcts_ = std::unique_ptr<MCTSBase>(
        new MCTS{getBoard(), Player::getByColor(Player::WHITE), selector, initializer, mover, scorer});
}

void StandardMCTSStrategy::update(Index idx) {
    Strategy::update(idx);

    mcts_->update(idx);
}

Index StandardMCTSStrategy::play() {
    auto idx = mcts_->simulate(300);

    std::cerr << "SCORES: " << std::endl;
    mcts_->print(5);

    mcts_->update(idx);
    return idx;
}
