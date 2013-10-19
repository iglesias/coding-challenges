#include <cstdlib>
#include <iostream>

int main(int argc, char** argv)
{
	int fib1 = 1, fib2 = 2;
	int bound = 4000000;
	// sum of the even terms of the Fibonacci sequence bounded
	int result = 0; // initialize to the sum of the first even number in the sequence

	if (argc > 1)
		bound = atoi(argv[1]);

	while (fib2 <= bound) {
		if (fib2%2 == 0)
			result += fib2;

		int save = fib2;
		fib2 += fib1;
		fib1 = save;
	}

	std::cout << result << std::endl;
}
