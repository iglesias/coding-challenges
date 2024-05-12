#include <algorithm>
#include <cstddef>
#include <limits>
#include <queue>
#include <ranges>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

using std::pair;
using std::priority_queue;
using std::vector;

double min_cost_to_hire_workers(const vector<int>& quality, const vector<int>& wage, int k) {
    vector<pair<double, int>> workers;
    workers.reserve(quality.size());
    for (std::size_t i = 0; i < quality.size(); i++)
        workers.emplace_back((1.0 * wage[i]) / quality[i], quality[i]);
    std::ranges::sort(workers);
    priority_queue<int> qualities;
    double ans = std::numeric_limits<double>::max(), sum = 0;
    for (const pair<double, int>& worker : workers) {
        sum += worker.second;
        qualities.push(worker.second);
        if (static_cast<int>(qualities.size()) > k) {
            sum -= qualities.top();
            qualities.pop();
        }
        if (static_cast<int>(qualities.size()) == k)
            ans = std::min(ans, worker.first * sum);
    }
    return ans;
}

constexpr double error_tolerance = 1e-5;

TEST(MinCostToHireWorkers, Case_1) {
    EXPECT_NEAR(min_cost_to_hire_workers({10, 20, 5}, {70, 50, 30}, 2), 105.0, error_tolerance);
}

TEST(MinCostToHireWorkers, Case_2) {
    EXPECT_NEAR(min_cost_to_hire_workers({3, 1, 10, 10, 1}, {4, 8, 2, 2, 7}, 3), 30.666666666667, error_tolerance);
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
