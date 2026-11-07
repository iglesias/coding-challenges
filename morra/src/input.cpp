#include "input.h"
#include "strategy.h"

#include <iostream>
#include <cstdlib>
#include <string>

namespace {

int handle_num_turns_input(int, char**);
std::string_view handle_strategy_input(int, char**);

}

auto morra::handle_input(int argc, char** argv) -> std::pair<int, std::string_view>
{
  return {handle_num_turns_input(argc, argv), 
          handle_strategy_input(argc, argv)};
}

namespace {

int handle_num_turns_input(int argc, char* argv[])
{
  const int default_num_turns = 5;
  int num_turns = default_num_turns;
  // If an argument is given, it is the number of times to play the game (n).
  if (argc > 1) {
    try {
      // If it is negative, use its absolute value.
      // If zero, use one instead.
      num_turns = std::max(1, std::abs(std::stoi(argv[1])));
    } catch(std::invalid_argument& error) {
      num_turns = default_num_turns;
    }
  }
  return num_turns;
}

std::string_view handle_strategy_input(int argc, char* argv[]) {
  if (argc < 3) {
    return std::string_view("random");
  } else {
    if (!morra::AVAILABLE_STRATEGIES.contains(argv[2])) {
      std::cerr << "Input strategy not available."
                << " The available strategies are:\n";
      size_t i = 0;
      for (auto const& available_strategy : morra::AVAILABLE_STRATEGIES) {
        i++;
        std::cerr << available_strategy;
        if (i == morra::AVAILABLE_STRATEGIES.size()) {
          std::cerr << "." << std::endl;
        } else if (i == morra::AVAILABLE_STRATEGIES.size() - 1) {
          std::cerr << " and ";
        } else {
          std::cerr << ", ";
        }
      }
      std::exit(0);
    }
    return argv[2];
  }
}

}  // anonymous namespace
