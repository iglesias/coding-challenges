#include <iostream>
#include <cstdlib>

int main(int argc, char** argv)
{
	int N = 1000;
	int result = 0;

	if (argc > 1)
		N = atoi(argv[1]);

	for (int i = 1; i < N; ++i) {
		if (i%3 == 0 || i%5 == 0)
			result += i;
	}

	std::cout << "Sum below " << N << " = " << result << std::endl;

	return 0;
}
