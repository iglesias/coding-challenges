#include <iostream>
#include <set>
#include <string>
#include <cassert>
#include <cstdlib>
#include <chrono>
#include <cstdio>
#include <vector>
// #include <pair>

std::set<std::string> GenBalanced(int n)
{
	assert(n%2 == 0);

	std::set<std::string> result;

	if (n == 2) {
		result.insert("()");
	} else if (n > 2) {
		std::set<std::string> aux = GenBalanced(n-2);
		for (auto it = aux.begin(); it != aux.end(); ++it) {
			result.insert("()" + *it);
			result.insert("(" + *it + ")");
			result.insert(*it + "()");
		}
	}

	return result;
}

bool IsBalanced(const std::string& str)
{
	// strings of parentheses with uneven length cannot be balanced
	if (str.size()%2 == 1) return false;

	int count = 0;
	for (unsigned int i = 0; i < str.length(); i++) {
		switch (str[i]) {
		case '(':
			count++;
			break;
		case ')':
			if (count <= 0)
				return false;
			else
				count--;

			break;
		default:
			// the string should only contain parentheses
			assert(false);
		}
	}

	return count == 0;
}

bool IsBalanced(const std::set<std::string>& s)
{
	for (auto it = s.begin(); it != s.end(); ++it)
		if (!IsBalanced(*it)) return false;

	return true;
}

template <class T>
void DisplaySet(const std::set<T>& s)
{
	for (auto it = s.begin(); it != s.end(); ++it)
		std::cout << *it << std::endl;
}

int main(int argc, char** argv)
{
	int bound = 0;

	if (argc > 1)
		bound = std::atoi(argv[1]);

	if (bound == 0) bound = 10;

	std::vector<std::pair<int, double>> log;
	std::chrono::time_point<std::chrono::system_clock> start, end;

	for (int n = 2; n < bound; n += 2) {
		start = std::chrono::system_clock::now();
		IsBalanced(GenBalanced(n));
		end = std::chrono::system_clock::now();
		double time = std::chrono::duration<double>(end-start).count();
		log.push_back(std::pair<int, double>(n, time));
	}

	for (unsigned int i = 0; i < log.size(); i++)
		std::cout << log[i].first <<  " ";
	std::cout << std::endl;

	for (unsigned int i = 0; i < log.size(); i++)
		std::cout << log[i].second << " ";
	std::cout << std::endl;

	return 0;
}
