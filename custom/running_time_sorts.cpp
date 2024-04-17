/*
 * 2013 Fernando J. Iglesias Garcia
 */

#include <numeric>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>

/* This program  performs a practical comparison of quick sort and insertion sort
 * running times. To do so, the number of swap operations performed by each algorithm
 * are counted.
 */

using namespace std;

// To count the number of swaps that insertion sort will require, we just need to count
// the number of elements larger than ar[i] that are in the array before element i.
int insertion_swaps(const vector<int>& ar)
{
	set<int> s;
	int count = 0;
	for (unsigned int i = 0; i < ar.size(); ++i)
	{
		// rank operation, could be done faster using an order statistics tree
		count += static_cast<int>(distance(s.upper_bound(ar[i]), s.end()));
		s.insert(ar[i]);
	}

	return count;
}

void swap(vector<int>& ar, int i, int j, int& count)
{
	int t = ar[i];
	ar[i] = ar[j];
	ar[j] = t;
	count++;
}

// The pivot is the last element in the subarray given to partition. The subarray
// is traversed, moving the elements that are smaller than the pivot into the left-most
// half of the subarray.
int partition(vector<int>& ar, int lo, int hi, int& count)
{
	int i = lo;
	for (int j = lo; j < hi; j++)
	{
		if (ar[j] < ar[hi])
			swap(ar, i++, j, count);
	}

	swap(ar, i, hi, count);
	return i;
}

void quicksort(vector<int>& ar, int lo, int hi, int& count)
{
	if (hi <= lo) return;
	int p = partition(ar, lo, hi, count);
	quicksort(ar, lo, p-1, count);
	quicksort(ar, p+1, hi, count);
}

int quicksort_swaps(vector<int> ar)
{
	int count = 0;
	quicksort(ar, 0, static_cast<int>(ar.size())-1, count);
	return count;
}

int main()
{
	int n;
	cin >> n;
	vector<int> ar(n);
  std::iota(ar.begin(), ar.end(), 0);

	cout << insertion_swaps(ar) - quicksort_swaps(ar) << endl;
	return 0;
}
