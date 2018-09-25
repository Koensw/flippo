#ifndef MCTS_H
#define MCTS_H

#include "../game/board.h"
#include "../game/player.h"
#include "node.h"

#include <algorithm>
#include <cassert>
#include <cmath>
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
        root_->ch.resize(root_->mvs.size());
    }

    Index simulate(unsigned int n) override {
        // simulate
        for(unsigned int i = 0; i < n; ++i) {
            select(root_.get());
        }

        // find best
        int best = -1;
        Index bidx;
        for(size_t i = 0; i < root_->mvs.size(); ++i) {
            if(root_->ch[i] == nullptr) continue;

            if(root_->ch[i]->n > best) {
                best = root_->ch[i]->n;
                bidx = root_->mvs[i];
            }
        }

        assert(best != -1);
        return bidx;
    }
    void update(Index idx) override {
        auto iter = std::find(root_->mvs.begin(), root_->mvs.end(), idx);
        if(iter != root_->mvs.end() && root_->ch[iter - root_->mvs.begin()] != nullptr) {
            root_ = std::move(root_->ch[iter - root_->mvs.begin()]);
        } else {
            Player pl = root_->pl;
            Board brd = root_->brd;
            brd.apply(idx, pl);
            root_ = std::make_unique<Node>();
            root_->pl = pl.opponent();
            root_->brd = brd;
            root_->mvs = root_->brd.getMoves(root_->pl);
            root_->ch.resize(root_->mvs.size());
        }
    }

    double select(Node* n) override {
        assert(n->ch.size() == n->mvs.size());
        // quit in final state
        if(n->brd.stones() == 64) {
            double score = n->s;
            if(!n->f) {
                score = n->s = scorer_(n->brd, n->pl.opponent());
                n->f = true;
            }
            n->w += score;
            n->n += 1;
            return score;
        }

        // choose
        Node* bn = nullptr;
        Index bnidx;
        int bni = 0;
        double best = std::numeric_limits<double>::lowest();
        for(size_t i = 0; i < n->mvs.size(); ++i) {
            double score = 0;
            score = selector_(n, n->ch[i].get());

            if(score > best) {
                best = score;
                bn = n->ch[i].get();
                bni = i;
                bnidx = n->mvs[i];
            }
        }

        // recurse
        double s = 0;
        if(bn == nullptr) {
            auto r = expand(n->brd, bnidx, n->pl);
            s = 1 - r->w;
            n->ch[bni] = std::move(r);
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
        n->ch.resize(n->mvs.size());
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
        for(size_t i = 0; i < root_->ch.size(); ++i) {
            if(root_->ch[i] == nullptr) continue;
            nds.push_back({root_->ch[i]->n, i});
        }

        std::sort(nds.begin(), nds.end(), std::greater<std::pair<int, int>>());

        for(unsigned int i = 0; i < std::min(n, static_cast<unsigned int>(nds.size())); ++i) {
            int j = nds[i].second;
            auto& n = root_->ch[j];
            std::cerr << Board::getString(root_->mvs[j]) << ": " << n->n << " (" << n->w / n->n << ")" << std::endl;
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
