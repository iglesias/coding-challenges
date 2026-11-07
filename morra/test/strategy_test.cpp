#include "strategy.h"

#include <gtest/gtest.h>
#include <stdexcept>

TEST(GetStrategyTest, InvalidStrategyThrows) {
  EXPECT_THROW(morra::make_strategy("winner"), std::invalid_argument);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
