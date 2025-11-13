#include <cassert>
#include <string>

int maxOperationsSlow(std::string s) {
    int ans = 0;
    std::string::size_type i = 0;
    while (i + 1 < s.length()) {
        while (i + 1 < s.length() && s[i] == '0') i++;
        if (i + 1 < s.length()) {
            assert(s[i] == '1');
            if (s[i + 1] == '1') {
                i++;
            } else {
                ans++;
                s[i] = '0';
                std::string::size_type j = i + 1;
                while (j < s.length() && s[j] == '0') j++;
                s[j - 1] = '1';
                if (i) i -= 1;
            }
        }
    }
    return ans;
}

int maxOperations(std::string const& s) {
    std::string::size_type i = 0;
    int ans = 0;
    int one_count = 0;
    while (i < s.length()) {
        while (i < s.length() && s[i] == '0') i++;
        while (i < s.length() && s[i] == '1') {
            one_count++;
            i++;
        }
        if (i < s.length()) {
            assert(s[i] == '0');
            ans += one_count;
        }
    }
    return ans;
}

/*

 i=4 s="1001011"
 i=0 s="0011011"
 i=3 s="0010111"
 i=2 s="0001111"

 Do not do both i=4 and i=3 before i=0

 i=0 s="0011101"
 i=4 s="0011011"
 i=3 s="0010111"
 i=2 s="0001111"

 */
