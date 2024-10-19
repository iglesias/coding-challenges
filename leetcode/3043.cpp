#include <algorithm>
#include <vector>

#include <gtest/gtest.h>

using std::vector;

struct node {
    node* next[10] = {};
};

int count_digits(int i) {
    int ans = 0;
    while (i > 0) {
        i /= 10;
        ans++;
    }
    return ans;
}

static int p10s[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};

node arr2tree(vector<int> const& arr) {
    node root;
    for (const int a : arr) {
        node* node_ptr = &root;
        const int n = count_digits(a);
        for (int i = 0; i < n; i++) {
            const int d = (a / p10s[n - i - 1]) % 10;
            if (!node_ptr->next[d]) node_ptr->next[d] = new node();
            node_ptr = node_ptr->next[d];
        }
    }
    return root;
}

int f(node root, int a) {
    const int n = count_digits(a);
    node* node_ptr = &root;
    int ans = 0;
    for (int i = 0; i < n; i++) {
        const int d = (a / p10s[n - i - 1]) % 10;
        if (!node_ptr->next[d]) return ans;
        ans++;
        node_ptr = node_ptr->next[d];
    }
    return ans;
}

void free(node* node_ptr) {
    for (int i = 0; i < 10; i++) if (node_ptr->next[i]) free(node_ptr->next[i]);
    delete node_ptr;
}

void free(node root) {
    vector<node*> nodes;
    for (int i = 0; i < 10; i++) if (root.next[i]) free(root.next[i]);
}

int longest_common_prefix(vector<int> const& arr1, vector<int> const& arr2) {
    node root = arr2tree(arr1);
    int ans = 0;
    for (const int a : arr2) ans = std::max(ans, f(root, a));
    free(root);
    return ans;
}

TEST(longest_common_prefix, warming_up) {
    const vector<int> arr1{1, 10, 100};
    const vector<int> arr2{1000};
    EXPECT_EQ(longest_common_prefix(arr1, arr2), 3); }

TEST(longest_common_prefix, four) {
    const vector<int> arr1{10987654, 123, 87654321, 654321};
    const vector<int> arr2{109876, 1234567, 87654321, 654};
    EXPECT_EQ(longest_common_prefix(arr1, arr2), 8); }

TEST(longest_common_prefix, five) {
    const vector<int> arr1{123987, 567890, 234567, 890123, 456789};
    const vector<int> arr2{908123, 456123, 789012, 234567};
    EXPECT_EQ(longest_common_prefix(arr1, arr2), 6); }

TEST(longest_common_prefix, six) {
    const vector<int> arr1{1248,364524,73264823,2937935};
    const vector<int> arr2{73249,94895,656324,239583,1249};
    EXPECT_EQ(longest_common_prefix(arr1, arr2), 3); }

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS(); }
