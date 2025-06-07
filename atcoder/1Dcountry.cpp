#include <algorithm>
#include <iostream>

using namespace std;

// Coordinates of the villages
int X[2 * 100000];
// and villagers.
int64_t Pcumulative[2 * 100000];

int main() {
    int N;
    cin >> N;
    for (int n = 0; n < N; n++) cin >> X[n];
    cin >> Pcumulative[0];
    for (int n = 1; n < N; n++) {
        cin >> Pcumulative[n];
        Pcumulative[n] += Pcumulative[n - 1];
    }
    int Q;
    cin >> Q;
    for (int q = 0; q < Q; q++) {
        int Lq, Rq;
        cin >> Lq >> Rq;
        const long int i = distance(X, lower_bound(X, X + N, Lq)) - 1;
        long int j = distance(X, lower_bound(X, X + N, Rq));
        if ((j == N) or (j < N and X[j] > Rq)) j--;
        cout << Pcumulative[j] - (i >= 0 ? Pcumulative[i] : 0) << '\n';
    }
}
