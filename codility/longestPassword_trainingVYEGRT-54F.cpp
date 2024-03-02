// -std=c++20 -lCatch2Maind

#include <string_view>

#include <catch2/catch_test_macros.hpp>

int max_pass_length(std::string_view string) {
    int ans = -1;
    int num_letters = 0, num_digits = 0;
    bool discard = false;
    for(const char c : string) {
        if(c == ' '){
            if(!discard && !(num_letters%2) && num_digits%2) {
                ans = std::max(ans, num_letters+num_digits);
            }
            discard = false;
            num_digits = num_letters = 0;
        } else if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')) {
            num_letters++;
        } else if ('0' <= c && c <= '9') {
            num_digits++;
        } else {
            discard = true;
        }
    }
    if(!discard && !(num_letters%2) && num_digits%2) {
        ans = std::max(ans, num_letters+num_digits);
    }
    return ans;
}

TEST_CASE("max_pass_length") {
    REQUIRE(max_pass_length("test 5 a0A pass007 ?xy1") == 7);
}
