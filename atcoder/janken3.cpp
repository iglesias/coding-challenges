#include <iostream>
#include <queue>
#include <string>
#include <tuple>
#include <unordered_map>

#include <boost/functional/hash.hpp>

using namespace std;

int main() {
    int N;
    std::cin >> N;
    std::string S;
    std::cin >> S;

    std::queue<std::tuple<int, char, int>> q;

    switch (S[0]) {
        case 'R':
            q.push({1, 'R', 0});
            q.push({1, 'P', 1});
            break;
        case 'P':
            q.push({1, 'P', 0});
            q.push({1, 'S', 1});
            break;
        case 'S':
            q.push({1, 'S', 0});
            q.push({1, 'R', 1});
    }

    int ans = 0;
    std::unordered_map<pair<int, char>, int, boost::hash<pair<int, char>>> visited;

    while (!q.empty()) {
        const auto [i, prev, wins] = q.front();
        q.pop();

        if (i == N) {
            ans = max(ans, wins);
            continue;
        }

        if (visited.contains({i, prev}) and visited[{i, prev}] >= wins )
            continue;
        visited[{i, prev}] = wins;

        switch (S[i]) {
            case 'R':
                if (prev != 'P')
                    q.push({i + 1, 'P', wins + 1});
                if (prev != 'R')
                    q.push({i + 1, 'R', wins});
                break;
            case 'P':
                if (prev != 'S')
                    q.push({i + 1, 'S', wins + 1});
                if (prev != 'P')
                    q.push({i + 1, 'P', wins});
                break;
            case 'S':
                if (prev != 'R')
                    q.push({i + 1, 'R', wins + 1});
                if (prev != 'S')
                    q.push({i + 1, 'S', wins});
        }
    }

    std::cout << ans << '\n';
}
