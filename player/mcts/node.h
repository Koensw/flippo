#ifndef NODE_H
#define NODE_H

#include <memory>

#include "../game/board.h"
#include "../game/player.h"

struct Node {
    Node() : v(0), w(0), n(0), f(false), s(0), pl(Player::me()) {}

    double v;

    double w; // wins for parent
    double n;

    bool f;
    double s; // final score

    std::vector<Index> mvs;
    // std::unique_ptr<Node> ch[64];
    std::vector<std::unique_ptr<Node>> ch;

    Player pl;
    Board brd;
};

#endif
