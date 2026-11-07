#include "game.h"

#include <algorithm>
#include <random>

auto morra::valid(int input) -> bool
{
    return std::ranges::find(morra::VALID_CHOICES, input) != morra::VALID_CHOICES.cend();
}

auto morra::random_choice() -> Choice
{
  // Random generation private to minimize source code.
  static std::uniform_int_distribution<> dist(1, 3);
  static std::random_device rd;
  return Choice(dist(rd)*10 + dist(rd));
}

namespace {

using namespace morra;

auto compute_turn_impl(Choice const& lhs, Choice const& rhs) -> int
{
    if (lhs.show == rhs.guess && lhs.guess == rhs.show) {
        return  0;
    } else if (lhs.show == rhs.guess) {
        return -1;
    } else if (lhs.guess == rhs.show) {
        return  1;
    } else {
        return 0;
    }
}

}  // anonymous namespace

auto morra::compute_turn(Choice const& lhs, Choice const& rhs) -> Outcome 
{
    return static_cast<Outcome>(::compute_turn_impl(lhs, rhs));
}
