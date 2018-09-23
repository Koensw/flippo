#include "player.h"

#include <cassert>
#include <iostream>

Player Player::players_[3];
Player::Color Player::me_;
Player::Color Player::them_;

void Player::init(Color col_me) {
    players_[static_cast<int>(Player::WHITE)] = Player(Player::WHITE);
    players_[static_cast<int>(Player::BLACK)] = Player(Player::BLACK);
    players_[static_cast<int>(Player::EMPTY)] = Player(Player::EMPTY);

    me_ = col_me;
    them_ = static_cast<Player::Color>(!static_cast<int>(col_me));
}

Player& Player::me() {
    return players_[static_cast<int>(Player::me_)];
}
Player& Player::them() {
    return players_[static_cast<int>(Player::them_)];
}

bool Player::isMe() {
    return color_ == me().color_;
}
bool Player::isThem() {
    return color_ == them().color_;
}

Player Player::opponent() {
    assert(color() != Player::EMPTY);
    return Player::getById(!id());
}

Player::Color Player::color() {
    return this->color_;
}
int Player::id() {
    return static_cast<int>(this->color_);
}

Player Player::getByColor(Player::Color color) {
    return getById(static_cast<int>(color));
}
Player Player::getById(int id) {
    assert(0 <= id && id <= 2);
    return players_[id];
}

bool operator==(Player p1, Player p2) {
    return p1.color_ == p2.color_;
}
