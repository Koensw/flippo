#ifndef MCTS_H
#define MCTS_H

#include "../game/board.h"
#include "../game/player.h"
#include "node.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>

class MCTSBase {
public:
    virtual Index simulate(unsigned int n) = 0;
    virtual void update(Index idx) = 0;

    virtual double select(Node* n) = 0;
    virtual std::unique_ptr<Node> expand(Board brd, Index idx, Player pl) = 0;
    virtual double rollout(Board brd, Player pl) = 0;

    virtual void print(unsigned int n) = 0;
};

template <typename Selector, typename Initializer, typename Mover, typename Scorer> class MCTS : public MCTSBase {
public:
    MCTS(Board brd, Player pl, Selector selector, Initializer initializer, Mover mover, Scorer scorer)
        : selector_(selector), initializer_(initializer), mover_(mover), scorer_(scorer) {
        root_ = std::make_unique<Node>();
        root_->pl = pl;
        root_->brd = brd;
        root_->mvs = root_->brd.getMoves(root_->pl);
    }

    Index simulate(unsigned int n) override {
        // simulate
        for(unsigned int i = 0; i < n; ++i) {
            select(root_.get());
        }

        // find best
        int best = -1;
        Index bidx;
        for(int i = 0; i < 64; ++i) {
            if(root_->ch[i] == nullptr) continue;

            if(root_->ch[i]->n > best) {
                best = root_->ch[i]->n;
                bidx = {i / 8, i % 8};
            }
        }

        assert(best != -1);
        return bidx;
    }
    void update(Index idx) override {
        // assert(root_->ch[idx.r * 8 + idx.c] != 0);
        if(root_->ch[idx.r * 8 + idx.c] != 0) {
            root_ = std::move(root_->ch[idx.r * 8 + idx.c]);
        } else {
            Player pl = root_->pl;
            Board brd = root_->brd;
            brd.apply(idx, pl);
            root_ = std::make_unique<Node>();
            root_->pl = pl.opponent();
            root_->brd = brd;
            root_->mvs = root_->brd.getMoves(root_->pl);
        }
    }

    double select(Node* n) override {
        // quit in final state (FIXME: optimize)
        if(n->brd.stones() == 64) {
            double score = 0.0;
            if(!n->f) {
                score = n->s = scorer_(n->brd, n->pl.opponent());
                n->f = true;
            } else {
                score = n->s;
            }
            n->w += score;
            n->n += 1;
            return score;
        }

        // choose
        Node* bn = nullptr;
        Index bnidx;
        double best = std::numeric_limits<double>::lowest();
        for(auto& idx : n->mvs) {
            int i = idx.r * 8 + idx.c;
            double score = 0;
            if(n->ch[i] == nullptr) {
                score = std::numeric_limits<double>::max();
            } else {
                score = selector_(n, n->ch[i].get());
            }

            if(score > best) {
                best = score;
                bn = n->ch[i].get();
                bnidx = idx;
            }
        }

        // std::cerr << bn << " " << bnidx.r << " " << bnidx.c << std::endl;

        // recurse
        double s = 0;
        if(bn == nullptr) {
            auto r = expand(n->brd, bnidx, n->pl);
            s = 1 - r->w;
            n->ch[bnidx.r * 8 + bnidx.c] = std::move(r);
        } else {
            s = 1 - select(bn);
        }
        assert(0.0 <= s && s <= 1.0);

        // backpropagate
        n->w += s;
        n->n += 1;

        return s;
    }
    std::unique_ptr<Node> expand(Board brd, Index idx, Player pl) override {
        // initialize
        auto n = std::make_unique<Node>();
        n->brd = std::move(brd);
        n->brd.apply(idx, pl);
        n->pl = pl.opponent();
        n->mvs = n->brd.getMoves(n->pl);
        initializer_(n.get());

        // rollout
        n->w = rollout(n->brd, n->pl);
        n->n += 1;
        return n;
    }
    double rollout(Board brd, Player pl) override {
        Player cpl = pl.opponent();
        while(true) {
            if(brd.stones() == 64) break;
            auto idx = mover_(brd, pl);
            brd.apply(idx, pl);
            pl = pl.opponent();
        }
        auto score = scorer_(brd, cpl);
        assert(0.0 <= score && score <= 1.0);
        return score;
    }

    void print(unsigned int n) {
        std::vector<std::pair<int, int>> nds;
        for(int i = 0; i < 64; ++i) {
            if(root_->ch[i] == nullptr) continue;
            nds.push_back({root_->ch[i]->n, i});
        }

        std::sort(nds.begin(), nds.end(), std::greater<std::pair<int, int>>());

        for(unsigned int i = 0; i < std::min(n, static_cast<unsigned int>(nds.size())); ++i) {
            int j = nds[i].second;
            auto& n = root_->ch[j];
            std::cerr << Board::getString({j / 8, j % 8}) << ": " << n->n << " (" << n->w / n->n << ")" << std::endl;
        }
    }

private:
    std::unique_ptr<Node> root_;

    Selector selector_;
    Initializer initializer_;
    Mover mover_;
    Scorer scorer_;
};

#endif
