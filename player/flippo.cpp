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

#include <cassert>
#include <climits>
#include <memory>

int main(int argc, char** argv) {
    std::cerr << "R KFT" << std::endl;

    // Initializer global seed
    rng.seed();

    // Initialize player, board and apply initial move
    std::string str;
    if(argc == 2 && std::string(argv[1]) == "s")
        str = "Start";
    else
        std::cin >> str;

    // Select strategy
    std::unique_ptr<Strategy> strategy = std::make_unique<SimpleStrategy>();

    // Start game
    if(str == "Start") {
        strategy->start(Player::WHITE);
    } else {
        strategy->start(Player::BLACK);
        strategy->update(Board::getIndex(str));
    }

    while(true) {
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
        if(str == "Quit")
            break;
        strategy->update(Board::getIndex(str));
    }
}
