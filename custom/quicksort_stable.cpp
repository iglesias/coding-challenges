/* A stable variant of quicksort that uses linear memory and prints the
 * result of sorting each subarray
 *
 * Try for instance the following input:
 * 7
 * 5 8 1 3 7 9 2
 *
 * Written (W) Fernando J. Iglesias Garcia, 2013
 * Quicksort problem from www.hackerrank.com
 */

#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

void print(const vector<int>& ar, int lo, int hi)
{
    for (int i = lo; i <= hi; i++)
        cout << ar[i] << ' ';
    cout << endl;
}

void swap(vector<int>& ar, int i, int j)
{
    int tmp = ar[i];
    ar[i] = ar[j];
    ar[j] = tmp;
}

int partition(vector<int>& ar, int lo, int hi)
{
    vector<int> smaller, larger;
    for (int i = lo+1; i <= hi; ++i)
    {
        if (ar[i] > ar[lo]) larger.push_back(ar[i]);
        else smaller.push_back(ar[i]);
    }
    
    int p = ar[lo];
    int j = lo;
    for (int i = 0; i < static_cast<int>(smaller.size()); ++i)
        ar[j++] = smaller[i];
    
    int pi = j; 
    ar[j++] = p;
    
    for (int i = 0; i < static_cast<int>(larger.size()); ++i)
        ar[j++] = larger[i];
        
    return pi;
}

void quicksort(vector<int>& ar, int lo, int hi)
{
    if (hi <= lo) return;
    
    int p = partition(ar, lo, hi);
    quicksort(ar, lo, p-1);
    quicksort(ar, p+1, hi);
    
    print(ar, lo, hi);
}

void quicksort(vector<int>&  ar)
{
    quicksort(ar, 0, static_cast<int>(ar.size())-1);
}

int main(void)
{
    int N;
    cin >> N;
    vector<int> ar(N);
    std::iota(ar.begin(), ar.end(), 0);
    quicksort(ar);
    return 0;
}
