#include "strategy.h"

#include <cassert>

Strategy::Strategy() {
    start_ = std::chrono::high_resolution_clock::now();
    pause_ = false;
}

void Strategy::start(Player::Color c) {
    Player::init(c);
    brd_.init();
    pl_ = Player::getByColor(Player::WHITE);
}

void Strategy::update(Index idx) {
    brd_.apply(idx, pl_);
    pl_ = pl_.opponent();
}

Index Strategy::step() {
    Index idx = play();
    Strategy::update(idx);
    return idx;
}

Board Strategy::getBoard() const {
    return brd_;
}

void Strategy::pause() {
    assert(!pause_);
    pause_start_ = std::chrono::high_resolution_clock::now();
    pause_ = true;
}
double Strategy::getElapsed() const {
    assert(!pause_);
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_).count() / 1000.0 - extra_;
    return duration;
}
void Strategy::resume() {
    assert(pause_);
    auto now = std::chrono::high_resolution_clock::now();
    extra_ += std::chrono::duration_cast<std::chrono::milliseconds>(now - pause_start_).count() / 1000.0;
    pause_ = false;
}
