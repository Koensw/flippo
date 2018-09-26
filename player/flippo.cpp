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
#include <fstream>

int main(int argc, char** argv) {
    std::string arg;
    if(argc >= 2) arg = argv[1];

    // Initializer global seed
    rng.seed();

    // Select strategy
    std::unique_ptr<Strategy> strategy;
    if(arg == "s") {
        std::cerr << "R KF-SIMPLE" << std::endl;
        strategy = std::make_unique<SimpleStrategy>();
    } else {
        std::cerr << "R KF-MCTS-STANDARD" << std::endl;
        strategy = std::make_unique<StandardMCTSStrategy>();
    }

    // Check if game has to be preloaded
    if(arg == "r") {
        // Preload game playing as the one to move afterwards
        assert(argc == 4);
        int mv = std::stoi(argv[3]);
        if((mv % 2) == 0) strategy->start(Player::WHITE);
        else strategy->start(Player::BLACK);
        
        std::fstream file(argv[2]);
        assert(file);
        std::string str;
        int cnt = 0;
        while(file >> str && cnt < mv) {
            if(str.size() != 2 || !('A' <= str[0] && str[0] <= 'H' && '0' <= str[1] && str[1] <= '8')) continue;
            strategy->update(Board::getIndex(str));
            ++cnt;
        }
        std::cerr << "PRELOADED:" << std::endl;
        strategy->getBoard().print();
    } else {    
        // Initialize player, board and apply initial move
        std::string str;
        if(arg == "t")
            str = "Start";
        else {
            strategy->pause();
            std::cin >> str;
            strategy->resume();
        }

        // Start game
        if(str == "Start") {
            strategy->start(Player::WHITE);
        } else {
            strategy->start(Player::BLACK);
            strategy->update(Board::getIndex(str));
        }
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
        std::string str;
        strategy->pause();
        std::cin >> str;
        strategy->resume();
        if(str.empty() || str == "Quit") break;
        strategy->update(Board::getIndex(str));
    }

    std::cerr << "TIME: " << strategy->getElapsed() << std::endl;
}
