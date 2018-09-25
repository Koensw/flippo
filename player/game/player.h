#ifndef PLAYER_H
#define PLAYER_H

#include <cassert>
#include <cstdint>
#include <vector>

class Index {
public:
    Index() : r(0), c(0) {}
    Index(char rs, char cs) : r(rs), c(cs) {}
    int r;
    int c;
};

inline bool operator==(Index i1, Index i2) {
    return i1.r == i2.r && i1.c == i2.c;
}

class Player {
    friend bool operator==(Player p1, Player p2);

public:
    enum Color {
        WHITE = 0,
        BLACK = 1,
        EMPTY = 2,
    };

    static void init(Color me);

    static inline Player me() { return players_[static_cast<int>(Player::me_)]; }
    static inline Player them() { return players_[static_cast<int>(Player::them_)]; }

    inline bool isMe() { return color_ == me().color_; }
    inline bool isThem() { return color_ == them().color_; }

    inline Player opponent() {
        assert(color() != Player::EMPTY);
        return Player::getById(!id());
    }

    inline Color color() { return this->color_; }
    inline int id() { return static_cast<int>(this->color_); }

    static inline Player getByColor(Player::Color color) { return getById(static_cast<int>(color)); }
    static inline Player getById(int id) {
        assert(0 <= id && id <= 2);
        return players_[id];
    }

private:
    Player() {}
    explicit Player(Color c) : color_(c) {}

    Color color_;

    static Player players_[3];

    static Color me_;
    static Color them_;
};

inline bool operator==(Player p1, Player p2) {
    return p1.color_ == p2.color_;
}

#endif
