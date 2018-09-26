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
        if(c == nullptr)
            scr = std::numeric_limits<double>::max()/2.0;
        else
            scr = c->w / c->n + 1.4 * std::sqrt(std::log(p->n) / c->n);

        scr += 1e-9 * (rng() % 1000);
        return scr;
    };
    auto initializer = [](Node* n) { n->v = 0; };
    auto mover = [](const Board& brd, Player pl) {
        (void)pl;
        auto mv = brd.getRandomBaseMove();
        return mv;
        /*auto mvs = brd.getMoves(pl);
        return mvs[rng() % mvs.size()];*/
    };
    auto scorer = [](const Board& brd, Player pl) {
        auto scr = (brd.count(pl) - 2.0) / 60.0;
        return scr;
    };
    
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
    
    // calculate new number of simulations
    auto second_per_sim = (end - start) / simulations_;
    //(64 - brd_.stones()) * new_sims * sim_per_second = PLAY_TIME;
    simulations_ = (PLAY_TIME - getElapsed()) / (((64 - brd_.stones() - 1)/2.0 + 1e-9) * second_per_sim);
    simulations_ = std::min(std::max(MIN_SIMULATIONS, simulations_), MAX_SIMULATIONS);
    
    // print debugging
    std::cerr << "NEXT: " << simulations_ << std::endl;
    std::cerr << "SCORES: " << std::endl;
    mcts_->print(5);

    // update
    mcts_->update(idx);
    return idx;
}
