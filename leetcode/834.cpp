#include <vector>

#include <gtest/gtest.h>

using namespace std;

namespace {

vector<vector<int>> A;   // adjacency list
vector<int> sizes, ans;  // sub-trees' sizes (#nodes inc. root), sum of distances

void init(const int n, const vector<vector<int>>& edges) {
    A = vector(n, vector<int>());
    for (const vector<int>& edge: edges) {
        A[edge[0]].push_back(edge[1]);
        A[edge[1]].push_back(edge[0]);
    }
    sizes.assign(n, 1);
    ans.resize(n);
}

void DFS(const int i = 0, const int parent = -1) {
    for (const int j : A[i]) {
        if (j == parent) continue;
        DFS(j, i);
        sizes[i] += sizes[j];
        ans[i]   += ans[j] + sizes[j];
    }
}

void reRoot(const int i = 0, const int parent = -1) {
    for (const int j : A[i]) {
        if (j == parent) continue;
        ans[j] = ans[i] + A.size() - 2 * sizes[j];
        reRoot(j, i);
    }
}

}  // namespace

vector<int> sumOfDistancesInTree(int n, const vector<vector<int>>& edges) {
    ::init(n, edges);
    ::DFS();
    ::reRoot();
    return ::ans;
}

TEST(sumOfDistancesInTree, Sample) {
    const vector<vector<int>> edges = {{0, 1}, {0, 2}, {2, 3}, {2, 4}, {2, 5}};
    EXPECT_EQ(sumOfDistancesInTree(6, edges), vector<int>({8, 12, 6, 10, 10, 10}));
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
