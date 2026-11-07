#include "game.h"

#include <algorithm>
#include <gtest/gtest.h>

using namespace morra;

TEST(ChoiceTest, UsesRandomChoice) {
    EXPECT_TRUE(std::ranges::find(VALID_CHOICES, random_choice().to_int()) != VALID_CHOICES.end());
}

TEST(MorraTest, ChecksMorraSchema) {
    using enum Outcome;

    EXPECT_EQ(compute_turn(11, 12), Win);
    EXPECT_EQ(compute_turn(11, 13), Win);
    EXPECT_EQ(compute_turn(12, 22), Win);
    EXPECT_EQ(compute_turn(12, 23), Win);
    EXPECT_EQ(compute_turn(13, 32), Win);
    EXPECT_EQ(compute_turn(13, 33), Win);

    EXPECT_EQ(compute_turn(21, 11), Win);
    EXPECT_EQ(compute_turn(21, 13), Win);
    EXPECT_EQ(compute_turn(22, 21), Win);
    EXPECT_EQ(compute_turn(22, 23), Win);
    EXPECT_EQ(compute_turn(23, 31), Win);
    EXPECT_EQ(compute_turn(23, 33), Win);

    EXPECT_EQ(compute_turn(31, 11), Win);
    EXPECT_EQ(compute_turn(31, 12), Win);
    EXPECT_EQ(compute_turn(32, 21), Win);
    EXPECT_EQ(compute_turn(32, 22), Win);
    EXPECT_EQ(compute_turn(33, 31), Win);
    EXPECT_EQ(compute_turn(33, 32), Win);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
