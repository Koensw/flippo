#include "player.h"

#include <cassert>

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
