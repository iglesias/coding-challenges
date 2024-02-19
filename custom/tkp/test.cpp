#include <gtest/gtest.h>

#include "solver.h"

TEST(solve, NoDeliveries)
{
  auto [_, route] = solve({}, {}, 3);
  EXPECT_TRUE(route.empty());
}

TEST(solve, NoPickups)
{
  std::vector<Event> deliveries;
  deliveries.emplace_back(0, 1, 3);
  auto [_, route] = solve(deliveries, {}, 3);
  EXPECT_TRUE(route.empty());
}

TEST(solve, OneAndOne)
{
  std::vector<Event> deliveries;
  deliveries.emplace_back(0, 1, 3);
  std::vector<Event> pickups;
  pickups.emplace_back(-1, -1, 1);
  const auto [n, route] = solve(deliveries, pickups, 3);
  EXPECT_EQ(n, 1);
  EXPECT_EQ(route.size(), 4);
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
