#pragma once

#include <string_view>
#include <unordered_set>
#include <variant>

#include "game.h"

using namespace std::literals;

namespace morra {

struct CycleStrategy {
    CycleStrategy() : mLastMove(11) {}
    Choice make_choice();
  private:
    Choice mLastMove;
};

struct RandomStrategy {
    Choice make_choice() const;
};

struct FixedStrategy {
    Choice make_choice() const;
};

using Strategy = std::variant<CycleStrategy, RandomStrategy, FixedStrategy>;

Strategy make_strategy(std::string_view strategy_choice);

const auto AVAILABLE_STRATEGIES =
    std::unordered_set{"cycle"sv, "random"sv, "fixed"sv};

}
