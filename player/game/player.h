#ifndef PLAYER_H
#define PLAYER_H

#include <cstdint>
#include <vector>

class Index {
public:
    Index() : r(0), c(0) {
    }
    Index(char rs, char cs) : r(rs), c(cs) {
    }
    int r;
    int c;
};

class Player {
    friend bool operator==(Player p1, Player p2);

public:
    enum Color {
        WHITE = 0,
        BLACK = 1,
        EMPTY = 2,
    };

    static void init(Color me);
    // FIXME: check reference vs value
    static Player& me();
    static Player& them();
    bool isMe();
    bool isThem();

    Player opponent();

    Color color();
    int id();

    static Player getByColor(Color color);
    static Player getById(int id);

private:
    Player() {
    }
    explicit Player(Color c) : color_(c) {
    }

    Color color_;

    static Player players_[3];

    static Color me_;
    static Color them_;
};

bool operator==(Player p1, Player p2);

#endif
