#include "board.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <unistd.h>

#include "../util/rand.h"
#include "../util/bit.h"

int rc[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
int cc[8] = {-1, 0, 1, 1, 1, 0, -1, -1};

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
    // Set main stone
    assert(pl.color() != Player::EMPTY && get(idx).color() == Player::EMPTY);
    set(idx, pl);

    // Flip stones (FIXME: optimize)
    int cnt = 0;
    for(int i = 0; i < 8; ++i) {
        int r = idx.r;
        int c = idx.c;
        int lr = r, lc = c;
        while(0 <= r && r < 8 && 0 <= c && c < 8) {
            Player cpl = get({r, c});
            if(cpl == pl) {
                lr = r;
                lc = c;
            } else if(cpl.color() == Player::EMPTY)
                break;
            r += rc[i];
            c += cc[i];
        }
        if(lr == idx.r && lc == idx.c) continue;
        r = lr;
        c = lc;
        r -= rc[i];
        c -= cc[i];
        while(r != idx.r || c != idx.c) {
            flip({r, c});
            cnt++;
            r -= rc[i];
            c -= cc[i];
        }
    }

    return cnt;
}

std::vector<Index> Board::getMoves(Player pl) const {
    // Find moves
    uint64_t opt = get_move_options(board_[0] | board_[1]);
    
    // Prune non flipping moves (if exists)
    std::vector<Index> mvs;
    bool flip_moves = false;
    while(opt) {
        unsigned int i = __builtin_ctzll(opt);
        opt ^= (1ull << i);
        Index idx = {i/8, i%8};
        
        Board cp = *this;
        if(cp.apply(idx, pl)) {
            if(!flip_moves) mvs.clear();
            flip_moves = true;
            mvs.push_back(idx);
        } else if(!flip_moves) {
            mvs.push_back(idx);
        }
    }
    return mvs;
}

// NOTE: does not take invalid non-flipping moves into account
Index Board::getRandomBaseMove() const {
    uint64_t opt = get_move_options(board_[0] | board_[1]);
    
    auto idx = (rng() % __builtin_popcountll(opt));
    int bp = select_bit(opt, idx);
    return {bp/8, bp%8};
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
    for(int i = 0; i < 8; ++i) std::cerr << i+1;
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
    for(int i = 0; i < 8; ++i) std::cerr << i+1;
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
