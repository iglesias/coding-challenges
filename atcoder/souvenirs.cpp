#include <bits/stdc++.h>

using namespace std;

int main()
{
    int N, M;
    cin >> N >> M;
    multiset<int> A;
    vector<int> B(M);
    for (int n = 0; n < N; n++) {
        int Ai;
        cin >> Ai;
        A.insert(Ai);
    }
    for (int m = 0; m < M; m++) {
        cin >> B[m];
    }
    ranges::sort(B);
    uint64_t ans = 0;
    for (const int Bi : B) {
        const auto lb = A.lower_bound(Bi);
        if (lb == A.cend()) {
            cout << -1 << '\n';
            return 0;
        }
        ans += *lb;
        A.erase(lb);
    }
    cout << ans << '\n';
}
