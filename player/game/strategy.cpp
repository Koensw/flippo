#include "strategy.h"

void Strategy::start(Player::Color c) {
    Player::init(c);
    brd_.init();
}

void Strategy::update(Index idx) {
    brd_.apply(idx, Player::them());
}

Index Strategy::step() {
    Index idx = play();
    brd_.apply(idx, Player::me());
    return idx;
}

Board Strategy::getBoard() const {
    return brd_;
}
