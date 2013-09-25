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

inline void swap(std::vector<float>& v, int a, int b)
{
	float t = v[a];
	v[a] = v[b];
	v[b] = t;
}

void insertionsort(std::vector<float>& v)
{
	for (int i = 1; i < int(v.size()); i++)
		for (int j = i; j > 0 && v[j] < v[j-1]; j--)
			swap(v, j, j-1);
}

void bucketsort(std::vector<float>& v)
{
	// number of buckets
	int N = v.size();
	// list of buckets
	std::vector< std::vector<float> > buckets(N);

	// put each element in v inside a bucket
	for (int i = 0; i < N; i++)
		buckets[int(std::floor(v[i]*N))].push_back(v[i]);

	// sort the buckets
	for (int i = 0; i < N; i++)
		std::sort(buckets[i].begin(), buckets[i].end());

	// traverse and fill in the sorted array
	int i = 0;
	for (int j = 0; j < N; j++)
	{
		for (unsigned int k = 0; k < buckets[j].size(); k++)
			v[i++] = buckets[j][k];
	}
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
	bucketsort(v0);
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
