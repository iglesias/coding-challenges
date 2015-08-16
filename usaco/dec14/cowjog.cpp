// http://usaco.org/index.php?page=viewproblem2&cpid=489

#include <fstream>
#include <iostream>
#include <vector>

typedef long long int ll;

using namespace std;

int main()
{
    ifstream fin;
    fin.open("cowjog.in");
    int N;
    fin >> N;
    vector<ll> vll(N);
    for (int i=0; i<N; i++)
    {
        ll p, v;
        fin >> p >> v;
        vll[i] = v;
    }
    fin.close();

    int idx = N-1;
    int ans = N;
    while (idx > 0)
    {
        ll v = vll[idx];
        while (--idx >= 0)
            if (vll[idx] > v)
                ans--;
            else
                break;
    }

    ofstream fout;
    fout.open("cowjog.out");
    fout << ans << endl;
    fout.close();

    return 0;
}
