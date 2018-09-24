/*
 * AI Player Blackhole Codecup
 *
 * (c) Koen Wolters 2016
 */

#include <algorithm>
#include <iostream>

#include "game/board.h"
#include "game/player.h"
#include "game/strategy.h"
#include "util/rand.h"

#include "strats/simple.h"
#include "strats/standard_mcts.h"

#include <cassert>
#include <climits>
#include <memory>

int main(int argc, char** argv) {
    std::string arg;
    if(argc == 2) arg = argv[1];

    // Initializer global seed
    // rng.seed();

    // Select strategy
    std::unique_ptr<Strategy> strategy;
    if(arg == "s") {
        std::cerr << "R KF-SIMPLE" << std::endl;
        strategy = std::make_unique<SimpleStrategy>();
    } else {
        std::cerr << "R KF-MCTS-STANDARD" << std::endl;
        strategy = std::make_unique<StandardMCTSStrategy>();
    }

    // Initialize player, board and apply initial move
    std::string str;
    if(arg == "t")
        str = "Start";
    else
        std::cin >> str;

    // Start game
    if(str == "Start") {
        strategy->start(Player::WHITE);
    } else {
        strategy->start(Player::BLACK);
        strategy->update(Board::getIndex(str));
    }

    while(strategy->getBoard().stones() != 64) {
#ifdef DEBUG
        std::vector<Index> mvs = strategy->getBoard().getMoves(Player::me());
        std::cerr << "MOVES (" << mvs.size() << "):";
        for(auto& idx : mvs) {
            std::cerr << " " << Board::getString(idx);
        }
        std::cerr << std::endl;
#endif

        // Calculate and play own move
        Index mov = strategy->step();
        std::cout << Board::getString(mov) << std::endl;

#ifdef DEBUG
        strategy->getBoard().print();
#endif

        // Retrieve opponent move and update board
        std::cin >> str;
        if(str.empty() || str == "Quit") break;
        strategy->update(Board::getIndex(str));
    }

    std::cerr << "TIME: " << strategy->getElapsed() << std::endl;
}
