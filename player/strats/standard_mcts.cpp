#include "standard_mcts.h"

#include "../mcts/mcts.hpp"
#include "../util/rand.h"

#include <climits>
#include <cmath>

#define PLAY_TIME 4.0
#define INIT_SIMULATIONS 8000
#define MIN_SIMULATIONS 128
#define MAX_SIMULATIONS 50000

void StandardMCTSStrategy::start(Player::Color c) {
    Strategy::start(c);

    auto selector = [](Node* p, Node* c) {
        double scr = 0.0;
        // UCB
        if(c == nullptr) {
            scr = 1e9;
        } else {
            scr = c->w / c->n + 0.1 * std::sqrt(std::log(p->n) / c->n);
        }

        // heuristic
        if(c != nullptr) scr += c->v / (c->n + 1);

        // random
        scr += 1e-9 * (rng() % 1000);
        return scr;
    };
    auto initializer = [](Node* n) {
        constexpr uint64_t corners = (1ull) | (1ull << 7ull) | (1ull << 56ull) | (1ull << 63ull);
        // Favor moves in a corner heuristically
        n->v = 0;
        n->v += 0.1 * __builtin_popcountll(corners & n->brd.bb(n->pl.opponent()));
    };
    auto mover = [](const Board& brd, Player pl) {
        (void)pl;
        auto rnd = rng() % 100;
        Index mv;
        // Favor more expensive fully checked moves with low number of stones
        if(rnd < (brd.stones() + 36)) {
            mv = brd.getRandomBaseMove();
        } else {
            mv = brd.getRandomMove(pl);
        }
        return mv;
    };
    auto scorer = [](const Board& brd, Player pl) {
        auto scr = (brd.count(pl) - 2.0) / 60.0;
        return scr;
    };

    total_simulations_ = 0;
    simulations_ = INIT_SIMULATIONS; // default number of simulation to start with (update dynamically)

    mcts_ = std::unique_ptr<MCTSBase>(
        new MCTS{getBoard(), Player::getByColor(Player::WHITE), selector, initializer, mover, scorer});
}

void StandardMCTSStrategy::update(Index idx) {
    Strategy::update(idx);

    mcts_->update(idx);
}

Index StandardMCTSStrategy::play() {
    auto start = getElapsed();
    auto idx = mcts_->simulate(simulations_);
    auto end = getElapsed();

    // calculate simulation total
    total_simulations_ += simulations_;

    // calculate new number of simulations
    auto second_per_sim = (end - start) / simulations_;
    simulations_ = (PLAY_TIME - getElapsed()) / (((64 - brd_.stones() - 1) / 2.0 + 1e-9) * second_per_sim);
    simulations_ = std::min(std::max(MIN_SIMULATIONS, simulations_), MAX_SIMULATIONS);

    // print debugging
    std::cerr << "STONES: " << simulations_ << " " << total_simulations_ << std::endl;
    std::cerr << "SCORES: " << std::endl;
    mcts_->print(5);

    // update
    mcts_->update(idx);
    return idx;
}
