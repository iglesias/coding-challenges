#pragma once

#include <array>
#include <string_view>

namespace morra {

// Checks whether input is valid (one of 11, 12, 13, 21, ..., 33)
bool valid(int input);

struct Choice {
    int show;
    int guess;

    Choice(int iChoice) : show(iChoice / 10), guess(iChoice % 10) {}
    constexpr auto to_int() { return show * 10 + guess; }
};

constexpr std::array<int, 9> VALID_CHOICES{11, 12, 13, 21, 22, 23, 31, 32, 33};

Choice random_choice();

using namespace std::literals;

constexpr std::string_view WELCOME_MESSAGE =
    "Welcome to a morra game!"sv;

// Each player does two things in a turn:
//   1) Choose 1, 2 or 3.
//   2) Guess the other player's choice.
constexpr std::string_view VALID_INPUT_HELP_MESSAGE =
    "Please enter one of 11, 12, 13, 21, 22, 23, 31, 32 or 33.\n"
    "The first digit encodes your choice, and the second, your guess."sv;

// From the real player's point of view.
enum struct Outcome { Loss = -1, Draw, Win };

Outcome compute_turn(Choice const&, Choice const&);

}
