/*
 * AI Player Blackhole Codecup
 * 
 * (c) Koen Wolters 2016
 */

#include <algorithm>
#include <iostream>

#include "util/rand.h"
#include "game/player.h"
#include "game/board.h"

#include <climits>
#include <cassert>

int main(int argc, char** argv){
    std::cerr << "R KFT" << std::endl;
    
    // Initializer global seed
    rng.seed();
    
    // Initialize player, board and apply initial move
    std::string str;
    if(argc == 2 && std::string(argv[1]) == "s") str = "Start";
    else std::cin >> str;
    
    Board brd;
    if(str == "Start") {
        Player::init(Player::WHITE);
        brd.init();
    } else {
        Player::init(Player::BLACK);
        brd.init();
        
        brd.apply(Board::getIndex(str), Player::them());
    }
    
    while(true) {
        // Get moves
        std::vector<Index> mvs = brd.getMoves(Player::me());
        
#ifdef DEBUG
        std::cerr << "MOVES (" << mvs.size() << "):";
        for(auto& idx : mvs) {
            std::cerr << " " << Board::getString(idx);
        }
        std::cerr << std::endl;
#endif
        
        // Choose a move
        std::vector<Index> bmvs;
        int best = INT_MIN;
        for(auto& idx : mvs) {
            Board bc = brd;
            bc.apply(idx, Player::me());
            int cnt = bc.count(Player::me()) - brd.count(Player::me());
            if(cnt > best) {
                best = cnt;
                bmvs.clear();
            }
            if(cnt == best) bmvs.push_back(idx);
        }
        
        int choice = rng() % bmvs.size();
        std::cout << Board::getString(bmvs[choice]) << std::endl;
        brd.apply(bmvs[choice], Player::me());
        
#ifdef DEBUG
        brd.print();
#endif
        
        // Retrieve opponent move
        std::cin >> str;
        if(str == "Quit") break;
        brd.apply(Board::getIndex(str), Player::them());
    }
    
    brd.print();
}
