#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

int main(int argc, char **argv) {
    const size_t num_days = [&](){
        try {
            const size_t num_days = argc > 1 ? std::stoi(argv[1]) : 18;
            return num_days;
        } catch (...) {
            std::cerr << "Usage: ./lanternfish [num_days]   with num_days a positive integer\n";
            std::exit(1);
        }
    }();
    std::vector<size_t> fishes(9);
    std::iota(fishes.begin(), fishes.end(), 0);
    constexpr size_t reset_days{6};
    constexpr size_t spawn_days{8};
    std::array<uint64_t, spawn_days + 1> hishes{};  // histogram of fishes' internal timers
    for (const int fish : fishes) hishes[fish]++;
    for (size_t day = 1; day <= num_days; day++) {
        uint64_t backup = hishes[0];
        //for (size_t i = 0; i < hishes.size() - 1; i++) hishes[i] = hishes[i + 1];
        std::rotate(hishes.begin(), hishes.begin() + 1, hishes.end());
        hishes[reset_days] += backup;
        hishes[spawn_days]  = backup;
    }
    std::cout << std::accumulate(hishes.cbegin(), hishes.cend(), 0ull) << "\n";
}
