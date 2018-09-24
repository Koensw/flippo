#include "strategy.h"

void Strategy::start(Player::Color c) {
    start_ = std::chrono::high_resolution_clock::now();
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

double Strategy::getElapsed() const {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_).count() / 1000.0;
    return duration;
}
