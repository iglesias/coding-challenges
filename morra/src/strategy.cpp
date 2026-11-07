#include "strategy.h"
#include "game.h"

#include <algorithm>
#include <iterator>
#include <stdexcept>

morra::Choice morra::RandomStrategy::make_choice() const {
  return morra::random_choice();
}

morra::Choice morra::FixedStrategy::make_choice() const {
  return morra::Choice(23);
}

morra::Choice morra::CycleStrategy::make_choice() {
    auto it = std::ranges::find(morra::VALID_CHOICES, mLastMove.to_int());
    std::advance(it, 1);
    if (it == morra::VALID_CHOICES.end()) it = morra::VALID_CHOICES.begin();
    mLastMove = Choice(*it);
    return mLastMove;
}

morra::Strategy morra::make_strategy(std::string_view strategy_choice) {
    if (strategy_choice == "cycle") {
        return CycleStrategy{};
    } else if (strategy_choice == "fixed") {
        return FixedStrategy{};
    } else if (strategy_choice == "random") {
        return RandomStrategy{};
    } else {
        throw std::invalid_argument("Invalid strategy choice (not one of cycle, fixed, or random).");
    }
}
