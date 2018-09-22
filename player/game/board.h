#ifndef BOARD_H
#define BOARD_H

#include "player.h"

#include <array>
#include <map>
#include <vector>
#include <cstdint>

class Board{
public:
    Board() { board_[0] = board_[1] = 0; }

    // Initialize board
    void init();
    
    // Board info
    Player get(Index);
    int count(Player);
    
    // Retrieve moves
    std::vector<Index> getMoves(Player);
    
    // Apply move
    int apply(Index idx, Player p);
    
    // Print board
    void print();
    
    // Index conversion functions
    static Index getIndex(std::string);
    static std::string getString(Index);
private:
    // Update board
    void set(Index, Player p);
    void flip(Index);
    
    uint64_t board_[2];
};

#endif
