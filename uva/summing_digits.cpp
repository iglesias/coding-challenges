#include <cstdint>

#include <gtest/gtest.h>

uint8_t sum_digits(uint64_t n)
{
	// base case -  1-digit numbers
	if (n / 10 == 0) return n;

	// recursion
	uint8_t digits_sum = 0;
	while (n > 0)
	{
		digits_sum += n%10;
		n /= 10;
	}

	return sum_digits(digits_sum);
}

TEST(sum_digits, SampleInput)
{
	EXPECT_EQ(sum_digits(2),          2);
	EXPECT_EQ(sum_digits(11),         2);
	EXPECT_EQ(sum_digits(47),         2);
	EXPECT_EQ(sum_digits(1234567892), 2);
}

TEST(sum_digits, LargestInput)
{
	EXPECT_EQ(sum_digits(UINT64_MAX), 6);
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
