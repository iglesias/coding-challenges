#include <cstdlib>
#include <ctime>
#include <iostream>

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    constexpr int R = 30;
    constexpr int C = 60;
    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            std::cout << std::rand() % 10;
        }
        std::cout << '\n';
    }
}
