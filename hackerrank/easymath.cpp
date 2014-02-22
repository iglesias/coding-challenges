#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>

using namespace std;

int count_digits(long long n)
{
    int num_digits = 0;
    while (n > 0)
    {
        num_digits++;
        n /= 10;
    }
    return num_digits;
}

long long next(long long n)
{
    cout << "next(" << n << ")" << endl;

    long long copy = n;
    // check if all the digits are 4
    while (copy > 0 && copy%10 == 4) copy /= 10;
    if (copy == 0)
    {
        int num_digits = count_digits(n);
        long long ret = 4;
        while (num_digits > 0)
        {
            num_digits--;
            ret *= 10;
        }
        
        return ret;
    }
    else
        assert(copy%10 == 0);
    
    // at least one digit is 0,
    // put a 4 in the first 0 found from the right
    long long multiplier = 1;
    copy = n;
    while (copy%10 == 4)
    {
        multiplier *= 10;
        copy /= 10;
    }
    
    assert(n/10 > multiplier);
    
    return n + 4*multiplier;
}

long long solve(long long n)
{
    int b = 0;
    while (n > 0 && n%10 == 0)
    {
        b++;
        n /= 10;
    }
    
    int a = 0;
    while (n > 0 && n%10 == 4)
    {
        a++;
        n /= 10;
    }
    
    return 2*a + b;
}

int main() {
    int T;
    long long X;
    cin >> T;
    for (int t = 0; t < T; t++)
    {
        cin >> X;
        long long n = 4;
        while (n < X)
        {
            n = next(n);
//            cout << ">>>>> " << n << endl;
        }
        
        while (n%X != 0)
        {
            n = next(n);
//            cout << ">>> " << n << endl;
        }
        
        cout << solve(n) << endl;
    }
    
    return 0;
}

