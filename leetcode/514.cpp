#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

using namespace std;

struct state {
    int c;
    int i;
    int n;
    state() : c(0), i(0), n(0) {}
    bool operator>(state const& other) const {
        if (c == other.c)
            return n > other.n;
        else
            return c > other.c;
    }
};

int l;

using lookup_t = vector<unordered_map<char, vector<pair<int, bool>>>>;

lookup_t preprocess(string_view ring) {
    lookup_t lookup = vector(l, unordered_map<char, vector<pair<int, bool>>>());
    for (int i = 0; i < l; i++) {
        // +1
        int j = (i + 1) % l;
        int n = 1;
        while (j != i) {
            lookup[i][ring[j]].emplace_back(n, true);
            n++;
            j = (j + 1) % l;
        }
        // -1
        j = i - 1; if (j < 0) j += l;
        n = 1;
        while (j != i) {
            lookup[i][ring[j]].emplace_back(n, false);
            n++;
            j--; if (j < 0) j += l;
        }
    }
    return lookup;
}

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2;  
    }
};

int findRotateSteps(string ring, string key) {
    l = static_cast<int>(ring.length());
    auto lookup = preprocess(ring);

    priority_queue<state, vector<state>, greater<state>> PQ;
    unordered_map<pair<int, int>, int, pair_hash> PQed;
    auto updateQs = [&](const state& s){
        if (PQed.contains(make_pair(s.c, s.i))) {
            if (PQed[make_pair(s.c, s.i)] > s.n) {
                PQ.emplace(s);
                PQed[make_pair(s.c, s.i)] = s.n;
            }
        } else {
            PQ.emplace(s);
            PQed.emplace(make_pair(s.c, s.i), s.n);
        }
    };
    updateQs(state{});

    while (!PQ.empty()) {
        state s{PQ.top()}; // FIXME: with queue::front -> state&
        if (s.c == static_cast<int>(key.length())) {
            return s.n;
        } else if (key[s.c] == ring[s.i]) {
            s.n++;
            s.c++;
            updateQs(s);
        } else {
            const int n0 = s.n;
            const int i0 = s.i;
            for (const auto& item : lookup[i0][key[s.c]]) {
                s.n = n0 + item.first;
                if (item.second)
                    s.i = (i0 + item.first) % l;
                else {
                    s.i = i0 - item.first;
                    if (s.i < 0) s.i += l;
                }
                updateQs(s);
            }
        }
        PQ.pop();
    }
    return -1;
}

TEST(FreedomTrail, case1) {
    ASSERT_EQ(findRotateSteps("godding", "gd"), 4);
}

TEST(FreedomTrail, case2) {
    ASSERT_EQ(findRotateSteps("godding", "godding"), 13);
}

TEST(FreedomTrail, testcase299) {
    ASSERT_EQ(findRotateSteps("erzjaclvkmqssenbrxmsyisgudxkurxgoxxqgxekexrcdcmggbgetiqhnhkabhdjsopkykdtsccvcylqoyjqotgrqtkibggulwlh", "hyoosmcpkliigxrkgrgxrymcsenbkdheqxamtkzbusekckicxtqhuwbcjaxsynhlrxxlrqutsdylgddteovvgsgocgqejqbgkgqj"), 832);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
