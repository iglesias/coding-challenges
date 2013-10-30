#include <iostream>
#include <set>
#include <string>
#include <cstdlib>

std::set<std::string> binary(int n, int k)
{
	std::set<std::string> result;

	if (n == 1) {
		switch(k) {
			case 0:
				result.insert("0");
				break;
			case 1:
				result.insert("1");
				break;
		};
	} else {
		auto aux1 = binary(n-1, k-1);
		for (auto it = aux1.begin(); it != aux1.end(); ++it)
			result.insert("1" + *it);

		auto aux2 = binary(n-1, k);
		for (auto it = aux2.begin(); it != aux2.end(); ++it)
			result.insert("0" + *it);
	}

	return result;
}


int main(int argc, char** argv)
{
	int n = 1;
	int k = 1;

	if (argc > 2) {
		n = std::atoi(argv[1]);
		k = std::atoi(argv[2]);
	}

	auto b = binary(n, k);

	for (auto it = b.begin(); it != b.end(); ++it)
		std::cout << *it << std::endl;

	return 0;
}
