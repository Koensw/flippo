#ifndef NODE_H
#define NODE_H

#include <memory>

struct Node {
    Node() : v(0), w(0), n(0), pl(Player::me()) {
        for(int i = 0; i < 64; ++i)
            ch[i] = 0;
    }

    double v;

    double w; // wins for parent
    double n;

    std::unique_ptr<Node> ch[64];

    Player pl;
    Board brd;
};

#endif
