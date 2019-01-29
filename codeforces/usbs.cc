// Started from https://github.com/technophilis/BigIntegerCPP/blob/master/BigIntegerSingleFile.cpp
// License of BigIntegerCPP follows.

/*
 * BigInteger Class, performs basic arithmetic operations of very large integers.
 * Copyright (C) 2011  Mahmoud Mechehoul
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <bits/stdc++.h>

using namespace std;

/* This is a minimized version of the BigInteger class meant to be
 * used for single file development purposes (such as programming contests).
 * For a complete documentation on what every method does please refer to
 * the class header file 'BigInteger.h'.
 */
class BigInteger {
private:
	string integer;
public:
	BigInteger(unsigned int integer);
	BigInteger(string integer);
	void setInteger(unsigned int integer);
	void setInteger(string integer);
	unsigned int getIntValue() const;
	string toString() const;
	BigInteger addInteger(const BigInteger& integer_to_add) const;
	BigInteger addInteger(const string& integer_to_add) const;
	BigInteger multiplyInteger(const BigInteger& integer_to_multiply) const;
	BigInteger multiplyInteger(const string& integer_to_multiply) const;
	static size_t getTrimIndex(const string& integer);
	bool operator==(const BigInteger& integer) const;
	BigInteger operator+(const BigInteger& integer) const;
	BigInteger operator*(const BigInteger& integer) const;
	friend ostream& operator<<(ostream& in, BigInteger& integer);
};

BigInteger::BigInteger(unsigned int integer) {
	setInteger(integer);
}

BigInteger::BigInteger(string integer) {
	for (int i = 0; i < (int)integer.size() && integer[i] >= '0' && integer[i] <= '9'; i++) {
		this->integer += integer[i];
	}

	if (this->integer.size() == 0) {
		this->integer = "0";
	} else {
		this->integer = integer.substr(getTrimIndex(integer));
	}
}

void BigInteger::setInteger(unsigned int integer) {
	if (integer == 0) this->integer = "0";

	while (integer) {
		this->integer = (char)((integer % 10) + '0') + this->integer;
		integer /= 10;
	}
}

void BigInteger::setInteger(string integer) {
	this->integer = integer;
}

unsigned int BigInteger::getIntValue() const {
	unsigned int ret = 0;
	unsigned int biggest = 0xFFFFFFFF;
	for (int i = 0; i < (int)integer.size(); i++) {
		int unit = integer[i] - '0';
		if (ret > (biggest - unit) / 10.0) return 0;
		ret = ret * 10 + unit;
	}
	return ret;
}

string BigInteger::toString() const {
	return integer;
}

BigInteger BigInteger::addInteger(const BigInteger& integer_to_add) const {
	int a_n = max((int)(integer_to_add.toString().size() - toString().size()), 0);
	int b_n = max((int)(toString().size() - integer_to_add.toString().size()), 0);
	string a = string(a_n, '0') + toString();
	string b = string(b_n, '0') + integer_to_add.toString();

	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());

	string result; int carry = 0;

	for (int i = 0; i < (int)a.size(); i++) {
	   int sum = (a[i] - '0') + (b[i] - '0') + carry;
	   result += ((char)(sum % 10 + '0'));
	   carry = sum / 10;
	}

	if (carry != 0) result += ((char)(carry + '0'));

	reverse(result.begin(), result.end());

	return BigInteger(result.substr(getTrimIndex(result)));
}

BigInteger BigInteger::addInteger(const string& integer_to_add) const {
	return addInteger(BigInteger(integer_to_add));
}

BigInteger BigInteger::multiplyInteger(const BigInteger& integer_to_multiply) const {
	string a = integer_to_multiply.toString();
	string b = toString();

	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());

	BigInteger ret("0");

	for (int i = 0; i < (int)a.size(); i++) {
		int carry = 0; string tmp = string(i, '0');

		for (int j = 0; j < (int)b.size(); j++) {
			int mul = (a[i] - '0') * (b[j] - '0') + carry;
			tmp += ((char)(mul % 10 + '0'));
			carry = mul / 10;
		}

		if (carry != 0) tmp += (carry + '0');

		reverse(tmp.begin(), tmp.end());

		ret = ret.addInteger(tmp.substr(getTrimIndex(tmp)));
	}

	return ret;
}

BigInteger BigInteger::multiplyInteger(const string& integer_to_multiply) const {
	return multiplyInteger(BigInteger(integer_to_multiply));
}

size_t BigInteger::getTrimIndex(const string& integer) {
	size_t index = 0;
	while (integer[index] == '0' && index < integer.size() - 1) index++;
	return index;
}

bool BigInteger::operator==(const BigInteger& integer) const {
	return this->integer == integer.toString();
}

BigInteger BigInteger::operator+(const BigInteger& integer) const {
	return addInteger(integer);
}

BigInteger BigInteger::operator*(const BigInteger& integer) const {
	return multiplyInteger(integer);
}

ostream& operator<<(ostream& in, BigInteger& integer) {
	in << integer.toString();

	return in;
}

vector<BigInteger> two_powers(unsigned int max_exponent)
{
	vector<BigInteger> powers(max_exponent+1, 1);
	BigInteger const two{2};

	for (unsigned int exponent{1}; exponent <= max_exponent; exponent++)
			powers[exponent] = powers[exponent-1]*two;
	
	return powers;
}

unsigned int const MAX_EXPONENT = 2000;

int main()
{
	unsigned int n;
	cin >> n;

	vector<pair<int,int>> sells(MAX_EXPONENT+1, make_pair(-1, -1));
	vector<int> wins(MAX_EXPONENT+1, -1);

	for (unsigned int i{0}; i < n; i++)
	{
		string action;
		unsigned int exponent;
		cin >> action >> exponent;

		if (action == "win")
		{
			wins[exponent] = i;
		}
		else if (action == "sell")
		{
			sells[exponent] = make_pair(wins[exponent], i);
		}
		else
		{
			cout << "Unexpected action\n";
			return 1;
		}
	}

	vector<int> intervals(n, 0); // 0 is free, 1 occupied.
	vector<int> sold_xs;
	for (int x = 2000; x >= 0; x--)
	{
		if (sells[x].first == -1) continue;

		bool can_sell{true};
		for (int i = sells[x].first; i <= sells[x].second && can_sell; i++)
			if (intervals[i]) can_sell = false;

		if (!can_sell) continue;

		for (int i = sells[x].first; i <= sells[x].second; i++)
			intervals[i] = 1;

		sold_xs.push_back(x);
	}

	BigInteger max_earnings{0};
	auto const powers{two_powers(MAX_EXPONENT)};
	for (int x : sold_xs) max_earnings = max_earnings + powers[x]; 
	std::cout << max_earnings << '\n';
}
