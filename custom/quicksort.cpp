#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>
#include <chrono>

std::random_device rd;
std::mt19937 gen(rd());

std::vector<float> uniform(int N)
{
	// uniform distribution in [0, 1)
	std::uniform_real_distribution<> uniform_dis;

	std::vector<float> random_numbers(N);
	for (int i = 0; i < N; i++)
		random_numbers[i] = uniform_dis(gen);

	return random_numbers;
}

template <class T>
void swap(std::vector<T>& v, int a, int b)
{
	T tmp = v[a];
	v[a] = v[b];
	v[b] = tmp;
}

template <class T>
int partition(std::vector<T>& v, int lo, int hi)
{
	int i = lo;
	int j = hi+1;
	
	while (true) {
		while (v[++i] < v[lo])
			if (i == hi) break;

		while (v[lo] < v[--j])
			if (j == lo) break;

		if (i >= j) break;

		swap(v, i, j);
	}

	swap(v, lo, j);
	return j;
}

template <class T>
void quicksort(std::vector<T>& v, int lo, int hi)
{
	if (hi <= lo) return;

	int p = partition(v, lo, hi);
	quicksort(v, lo, p-1);
	quicksort(v, p+1, hi);
}

template <class T>
void quicksort(std::vector<T>& v)
{
	// FIXME random shuffle
	quicksort(v, 0, v.size()-1);
}

int main(int argc, char** argv)
{
	// read length of the array
	int N = 0;
	if (argc > 1)
		N = std::atoi(argv[1]);

	// assign default value
	if (N == 0)
		N = 1e3;

	// generate array and sort it
	std::vector<float> v0 = uniform(N);
	// copy to sort with STL sort
	std::vector<float> v1(v0);

	std::chrono::time_point<std::chrono::system_clock> start, end; 

	start = std::chrono::system_clock::now();
	quicksort(v0);
	end = std::chrono::system_clock::now();
	std::cout << std::chrono::duration<double>(end-start).count() << std::endl;

	start = std::chrono::system_clock::now();
	std::sort(v1.begin(), v1.end());
	end = std::chrono::system_clock::now();
	std::cout << std::chrono::duration<double>(end-start).count() << std::endl;

	assert(v0.size() == v1.size());
	for (unsigned int i = 0; i < v0.size(); i++)
		assert(v0[i] == v1[i]);

	return 0;
}
