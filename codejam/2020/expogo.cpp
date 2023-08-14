#include <bits/stdc++.h>

using namespace std;

// solve test case
string solve()
{
    int X, Y;
    cin >> X >> Y;
    queue<tuple<int, int, int, string>> q;
    auto init_state = make_tuple(0,0,0,"");
    q.push(init_state);
    set<tuple<int, int, int, string>> vis;
    int num_iter = 0;
    while (!q.empty())
    {
        auto cur_state = q.front();
        q.pop();

//        cout << "(" << get<0>(cur_state) << ", " << get<1>(cur_state) << ")\n";
        if (get<0>(cur_state) == X && get<1>(cur_state) == Y)
          return get<3>(cur_state);

        if (vis.find(cur_state) != vis.end()) continue;

        num_iter++;
        if (num_iter > 500000) break;

        q.emplace(get<0>(cur_state) + (1<<get<2>(cur_state)), get<1>(cur_state), get<2>(cur_state)+1, get<3>(cur_state)+"E");
        q.emplace(get<0>(cur_state) - (1<<get<2>(cur_state)), get<1>(cur_state), get<2>(cur_state)+1, get<3>(cur_state)+"W");
        q.emplace(get<0>(cur_state), get<1>(cur_state) + (1<<get<2>(cur_state)), get<2>(cur_state)+1, get<3>(cur_state)+"N");
        q.emplace(get<0>(cur_state), get<1>(cur_state) - (1<<get<2>(cur_state)), get<2>(cur_state)+1, get<3>(cur_state)+"S");

        vis.insert(cur_state);
    }

    return "IMPOSSIBLE";
}

int main()
{
    int T;
    cin >> T;
    for (int t = 0; t < T; t++)
    {
      string ans = solve();
      cout << "Case #" << t+1 << ": " << ans << '\n';
    }
}

/*
4
2 3
-2 -3
3 0
-1 1
*/
