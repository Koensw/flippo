#include "simple.h"

#include "../util/rand.h"

#include <climits>
#include <iostream>

Index SimpleStrategy::play() {
    std::vector<Index> mvs = brd_.getMoves(Player::me());

    // Choose a move
    std::vector<Index> bmvs;
    int best = INT_MIN;
    for(auto& idx : mvs) {
        Board bc = brd_;
        bc.apply(idx, Player::me());
        int cnt = bc.count(Player::me()) - bc.count(Player::them());
        if(cnt > best) {
            best = cnt;
            bmvs.clear();
        }
        if(cnt == best) bmvs.push_back(idx);
    }

    int choice = rng() % bmvs.size();
    return bmvs[choice];
}
