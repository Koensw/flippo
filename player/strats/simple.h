#ifndef SIMPLE_STRATEGY_H
#define SIMPLE_STRATEGY_H

#include "../game/strategy.h"

class SimpleStrategy : public Strategy {
    Index play() override;
};

#endif
