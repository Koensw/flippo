#ifndef BOARD_H
#define BOARD_H

#include "player.h"

#include <array>
#include <cstdint>
#include <map>
#include <vector>

class Board {
public:
    Board() { board_[0] = board_[1] = 0; }

    // Initialize board
    void init();

    // Board info
    Player get(Index) const;
    
    inline int count(Player pl) const {
        assert(pl.color() != Player::EMPTY);
        return __builtin_popcountll(board_[pl.id()]);
    }
    inline int stones() const {
        return count(Player::me()) + count(Player::them());
    }

    // Retrieve moves
    std::vector<Index> getMoves(Player) const;
    Index getRandomBaseMove() const;

    // Apply move
    int apply(Index idx, Player p);

    // Print board
    void print() const;
    static void print(uint64_t board);

    // Index conversion functions
    static Index getIndex(std::string);
    static std::string getString(Index);

private:
    // Update board
    void set(Index, Player p);
    void flip(Index);
    
    // Find move options
    inline constexpr uint64_t get_move_options(uint64_t brd) const {
        uint64_t edge = ((brd & 18374403900871474942ull) >> 1) |
                        ((brd & 9187201950435737471ull) << 1) |
                        (brd << 8) | (brd >> 8);
        return edge & (~brd);
    }

    uint64_t board_[2];
};

#endif
