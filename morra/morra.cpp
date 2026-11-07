#include <cstdio> // for stderr
#include <iostream>
#include <print>
#include <variant>

#include "game.h"
#include "input.h"
#include "strategy.h"

int main(int argc, char* argv[])
{
    // User input validation and command-line argument handling
    auto [num_turns, strategy_choice] = morra::handle_input(argc, argv);
    auto strategy = morra::make_strategy(strategy_choice);

    std::println(morra::WELCOME_MESSAGE);
    std::println("We'll be playing {} turn{}.", num_turns, num_turns > 1 ? "s" : "");
    std::println(morra::VALID_INPUT_HELP_MESSAGE);

    // Game loop
    std::visit([num_turns](auto& visited_strategy) {
        for (int i = 0; i < num_turns; i++) {
            int input;
            std::cin >> input;
            if (!morra::valid(input)) {
                std::println(stderr, "Oh, {} isn't valid.", input);
                std::println(stderr, morra::VALID_INPUT_HELP_MESSAGE);
                i--;
                continue;
            }

            morra::Choice const player{input};
            morra::Choice const computer = visited_strategy.make_choice();

            std::print("You've shown {} finger{} and guessed {} ",
                       player.show, player.show > 1 ? "s" : "", player.guess);
            std::println("and I {} finger{} and guessed {}.",
                         computer.show, computer.show > 1 ? "s" : "", computer.guess);

            std::print("Turn {}: ", i+1);
            auto const outcome{compute_turn(player, computer)};
            if(outcome == morra::Outcome::Draw)
                std::println("Draw!");
            else if(outcome == morra::Outcome::Win)
                std::println("You win!");
            else //Outcome::Loss
                std::println("I win!");
        }
    }, strategy);
}
