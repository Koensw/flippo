#include "board.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <unistd.h>

#include "../util/bit.h"
#include "../util/debug.h"
#include "../util/rand.h"

int rc[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
int cc[8] = {-1, 0, 1, 1, 1, 0, -1, -1};

#include "board_bit.h"

void Board::init() {
    // Clear board
    board_[0] = board_[1] = 0;
    // Set center pieces
    set(getIndex("D4"), Player::getByColor(Player::WHITE));
    set(getIndex("E4"), Player::getByColor(Player::BLACK));
    set(getIndex("E5"), Player::getByColor(Player::WHITE));
    set(getIndex("D5"), Player::getByColor(Player::BLACK));
}

int Board::apply(Index idx, Player pl) {
    assert(pl.color() != Player::EMPTY && get(idx).color() == Player::EMPTY);

    // Find flip stones
    // FIXME: optimize
    uint64_t bc = (board_[0] | board_[1]);
    uint64_t bf = 0;
    for(int i = 0; i < 8; ++i) {
        int o = (i + 4) % 8;
        uint64_t b = get_direction(board_[pl.id()], o) & bc;
        uint64_t bn = (1ull << (idx.r * 8 + idx.c));

        int x = 0;
        if((i % 4) == 0)
            x = idx.r;
        else if((i % 4) == 1)
            x = (7 - idx.r) + idx.c;
        else if((i % 4) == 2)
            x = idx.c;
        else
            x = idx.r + idx.c;
        b &= bitl_tb[i % 4][x];

        for(int j = 0; j < 6; ++j) {
            bn |= (get_direction(bn, i) & bc);
            b |= (get_direction(b, o) & bc);
        }
        bf |= b & bn;
    }

    // Flip stones
    board_[0] ^= bf;
    board_[1] ^= bf;

    // Put new stone
    set(idx, pl);

    return __builtin_popcountll(bf);
}

uint64_t Board::getBitMoves(Player pl) const {
    uint64_t bc = board_[0] | board_[1];
    // Find base options
    uint64_t opt = get_move_options(bc);

    // Find allowed options
    uint64_t ba = 0;
    for(int i = 0; i < 8; ++i) {
        uint64_t b = get_direction(board_[pl.id()], i) & bc;
        for(int j = 0; j < 5; ++j) {
            b |= get_direction(b, i) & bc;
        }
        ba |= get_direction(b, i);
    }

    // Remove non flipping moves (if exists)
    if(ba & opt) opt &= ba;

    return opt;
}

std::vector<Index> Board::getMoves(Player pl) const {
    // Get moves
    uint64_t opt = getBitMoves(pl);

    // Generate move list
    std::vector<Index> mvs;
    while(opt) {
        int i = __builtin_ctzll(opt);
        opt ^= (1ull << i);
        Index idx = {i / 8, i % 8};

        mvs.push_back(idx);
    }
    return mvs;
}

Index Board::getRandomMove(Player pl) const {
    uint64_t opt = getBitMoves(pl);

    auto idx = (rng() % __builtin_popcountll(opt));
    int bp = select_bit(opt, idx);
    return {bp / 8, bp % 8};
}

// NOTE: does not take invalid non-flipping moves into account
Index Board::getRandomBaseMove() const {
    uint64_t opt = get_move_options(board_[0] | board_[1]);

    auto idx = (rng() % __builtin_popcountll(opt));
    int bp = select_bit(opt, idx);
    return {bp / 8, bp % 8};
}

Player Board::get(Index idx) const {
    if(board_[0] & (1ll << (8 * idx.r + idx.c)))
        return Player::getById(0);
    else if(board_[1] & (1ll << (8 * idx.r + idx.c)))
        return Player::getById(1);
    else
        return Player::getById(2);
}

void Board::print() const {
    std::cerr << " ";
    for(int i = 0; i < 8; ++i)
        std::cerr << i + 1;
    std::cerr << std::endl;
    for(int i = 0; i < 8; ++i) {
        std::cerr << static_cast<char>('A' + i);
        for(int j = 0; j < 8; ++j) {
            if(board_[Player::WHITE] & (1ll << (i * 8 + j)))
                std::cerr << "W";
            else if(board_[Player::BLACK] & (1ll << (i * 8 + j)))
                std::cerr << "B";
            else
                std::cerr << ".";
        }
        std::cerr << std::endl;
    }
}
void Board::print(uint64_t brd) {
    std::cerr << " ";
    for(int i = 0; i < 8; ++i)
        std::cerr << i + 1;
    std::cerr << std::endl;
    for(int i = 0; i < 8; ++i) {
        std::cerr << static_cast<char>('A' + i);
        for(int j = 0; j < 8; ++j) {
            if(brd & (1ll << (i * 8 + j)))
                std::cerr << "#";
            else
                std::cerr << ".";
        }
        std::cerr << std::endl;
    }
}

Index Board::getIndex(std::string str) {
    assert(str.size() == 2 && isalpha(str[0]) && 'A' <= str[0] && str[0] <= 'H' && isdigit(str[1]) && '0' <= str[1] &&
           str[1] <= '8');
    return {str[0] - 'A', str[1] - '1'};
}
std::string Board::getString(Index idx) {
    std::string s;
    s += 'A' + idx.r;
    s += '1' + idx.c;
    return s;
}

void Board::set(Index idx, Player pl) {
    board_[0] &= ~(1ll << (8 * idx.r + idx.c));
    board_[1] &= ~(1ll << (8 * idx.r + idx.c));
    if(pl.color() != Player::EMPTY) board_[pl.id()] |= (1ll << (8 * idx.r + idx.c));
}
void Board::flip(Index idx) {
    board_[0] ^= (1ll << (8 * idx.r + idx.c));
    board_[1] ^= (1ll << (8 * idx.r + idx.c));
}
