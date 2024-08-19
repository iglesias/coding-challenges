#include <exception>
#include <iostream>
#include <string>
#include <tuple>
#include <queue>
#include <unordered_set>

struct hash {
    std::size_t operator()(const std::tuple<int, int, int> &t) const {
        auto h0 = std::hash<int>{}(std::get<0>(t));
        auto h1 = std::hash<int>{}(std::get<1>(t));
        auto h2 = std::hash<int>{}(std::get<2>(t));
        return h0 ^ h1 ^ h2;
    }
};

int min_steps(int n) {
    std::queue<std::tuple<int, int, int>> Q;
    Q.emplace(1, 0, 0);
    std::unordered_set<std::tuple<int, int, int>, hash> Qed;
    Qed.emplace(1, 0, 0);
    while (!Q.empty()) {
        const auto [a, b, c] = Q.front();
        std::cout << ">> " << a << ' ' << b << ' ' << c << std::endl;
        if (a == n) return c;
        if (a > n) {
            Q.pop();
            continue;
        }
        if (b != a && !Qed.contains({a, a, c + 1})) {
            Qed.emplace(a, a, c + 1);
            Q.emplace(a, a, c + 1);
        }
        if (b != 0 && (a + b <= n) && !Qed.contains({a + b, b, c + 1})) {
            Qed.emplace(a + b, b, c + 1);
            Q.emplace(a + b, b, c + 1);
        }
        Q.pop();
    }
    return -1;
}

int main(int argc, char** argv) {
    const int n = (argc <= 1 ? 10 : [argv](){
        try {
            const int x = std::stoi(argv[1]);
            if (x < 1) throw std::exception{};
            return x;
        } catch (...) {
            std::cerr << "Optional input argument must be an integer greater than zero.\n";
            std::exit(1);
        }}());
    std::cout << min_steps(n) << '\n';
}
