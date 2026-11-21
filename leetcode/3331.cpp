#include <array>
#include <cassert>
#include <stack>
#include <string>
#include <utility>
#include <vector>

using std::array;
using std::stack;
using std::string;
using std::vector;

constexpr int NUM_CHARS = 'z'-'a'+1;

void DFS(const int x, const vector<int>& parent, const string& s, const vector<vector<int>>& children, array<stack<int>, NUM_CHARS>& A, vector<int>& b) {
    if (!A[s[x]-'a'].empty()) {
        b[x] = A[s[x]-'a'].top();
    }
    A[s[x]-'a'].push(x);
    for (const int z : children[x]) {
        DFS(z, parent, s, children, A, b);
    }
    A[s[x]-'a'].pop();
}

int POSTORDER(const int x, const vector<vector<int>>& children, vector<int>& ans) {
    int acc = 0;
    for (const int z : children[x]) {
        acc += POSTORDER(z, children, ans);
    }
    ans[x] = acc + 1;
    return acc + 1;
}

vector<int> findSubtreeSizes(vector<int>& parent, string s) {
    const int n = parent.size();
    // Make children from parent:
    vector<vector<int>> children(n, vector<int>{});
    assert(parent[0] == -1);
    for (int i = 1; i < n; i++) {
        children[parent[i]].push_back(i);
    }
    // Initialize characters to (ancestor) nodes
    array<stack<int>, NUM_CHARS> A;
    A[s[0]-'a'].push(0);
    // Search
    vector<int> b(n, -1);
    for (const int x : children[0]) {
        DFS(x, parent, s, children, A, b);
    }
    /*
    for (const int bi : b) {
        cout << bi << ' ';
    }
    cout << endl;
    for (const int bi : parent) {
        cout << bi << ' ';
    }
    cout << endl;
    */
    for (int i = 0; i < n; i++) {
        if (b[i] != -1 && b[i] != parent[i]) {
            parent[i] = b[i];
        }
    }
    /*
    for (const int bi : parent) {
        cout << bi << ' ';
    }
    cout << endl;
    */
    vector<int> ans(n, 0);
    children = vector<vector<int>>(n, vector<int>{});
    assert(parent[0] == -1);
    for (int i = 1; i < n; i++) {
        assert(0 <= parent[i] && parent[i] < n);
        children[parent[i]].push_back(i);
    }
    std::ignore = POSTORDER(0, children, ans);
    return ans;
}

// traverse from root in BFS or DFS storing seen characters to last node
// where it was seen,
// when visiting a node, if its character has been seen it means an
// ancestor has the same character, so store it for the transformation
// later
// postorder traversal to count subtree sizes

int main() {}
