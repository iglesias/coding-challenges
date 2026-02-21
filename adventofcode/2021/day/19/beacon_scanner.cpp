#include <array>
#include <bitset>
#include <cassert>
#include <functional>
#include <iomanip>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <unordered_set>

#include <boost/functional/hash.hpp>

struct symbol {
    std::bitset<2> id;
    bool negative;
    symbol& operator-() {
        negative = !negative;
        return *this;
    }
    bool operator==(symbol const& that) const {
      return id == that.id && negative == that.negative;
    }
    bool operator==(char c) const {
        switch (c) {
        case 'a':
            return id == std::bitset<2>(0);
        case 'b':
            return id == std::bitset<2>(1);
        case 'c':
            return id == std::bitset<2>(2);
        default:
            throw std::invalid_argument("Invalid character for symbol. It isn't one of a, b, or c.");
        }
    }
};

std::size_t hash_value(symbol const& s) {
    return s.id.to_ullong() ^ s.negative;
}

void print(symbol const& s) {
    std::cout << (s.negative ? '-' : ' ');
    switch (s.id.to_ulong()) {
    case 0:
        std::cout << 'a';
        break;
    case 1:
        std::cout << 'b';
        break;
    case 2:
        std::cout << 'c';
        break;
    default:
        throw std::runtime_error("Unexpected symbol codification.");
    }
}

void print(std::array<symbol, 3> const& a) {
    for (auto const& s : a) {
        print(s);
        std::cout << ' ';
    }
}

void print(std::array<std::array<int, 3>, 3> matrix) {
    for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++)
        std::cout << std::setw(2) << matrix.at(i).at(j) << (j == 2 ? "\n" : " ");
}

// [ 1       0          0     ]
// [ 0  cos(alpha) sin(alpha) ]
// [ 0 -sin(alpha) cos(alpha) ]
template<typename T> void Rx(std::array<T, 3>& v) {
    T backup = v[1];
    v[1] = v[2];
    v[2] = -backup;
}

// [ cos(beta) 0 -sin(beta) ]
// [      0    1       0    ]
// [ sin(beta) 0  cos(beta) ]
template<typename T> void Ry(std::array<T, 3>& v) {
    T backup = v[0];
    v[0] = -v[2];
    v[2] = backup;
}

// [  cos(gamma) sin(gamma) 0 ]
// [ -sin(gamma) cos(gamma) 0 ]
// [       0          0     1 ]
template<typename T> void Rz(std::array<T, 3>& v) {
    T backup = v[0];
    v[0] = v[1];
    v[1] = -backup;
}

// vector [a b c]^T is used as "canonical".
std::array<symbol, 3> const vector = [](){
        std::array<symbol, 3> vector;
        vector.at(0).id = 0, vector.at(0).negative = false;
        vector.at(1).id = 1, vector.at(1).negative = false;
        vector.at(2).id = 2, vector.at(2).negative = false;
        return vector;
    }();

auto generate_rotations() {
    std::unordered_set<std::array<symbol, 3>, boost::hash<std::array<symbol, 3>>> Qed;
    Qed.insert(vector);
    std::queue<std::array<symbol, 3>> Q;
    Q.push(vector);
    auto g = [&Q, &Qed](auto const& v, std::function<void(std::array<symbol, 3>&)> R) {
        auto w = v;
        R(w);
        if (!Qed.contains(w)) {
            Q.push(w);
            Qed.insert(w);
        }
    };
    while (!Q.empty()) {
        std::array<symbol, 3> const v = Q.front();
        Q.pop();
        g(v, Rx<symbol>);
        g(v, Ry<symbol>);
        g(v, Rz<symbol>);
    }
    if (Qed.size() != 24) throw std::runtime_error("Unexpected #rotations.");
    return Qed;
}

auto rotation_vector_mapping(std::array<symbol, 3> const& item) {
    auto find_symbol_position = [](auto const& item, char c){
        for (int pos = 0; pos < 3; pos++) if (item.at(pos) == c) return pos;
        throw std::runtime_error("Symbol not found.");
    };

    std::array<std::array<int, 3>, 3> matrix;
    matrix.fill(std::array<int, 3>{0, 0, 0});
    for (int offset = 0; offset < 3; offset++) {
        int const pos = find_symbol_position(item, 'a'+offset);
        matrix.at(offset).at(pos) = item.at(pos).negative ? -1 : 1;
    }
    return matrix;
}

int main() {
    auto const rotations = generate_rotations();
    for (auto const& item : rotations) {
        auto const matrix = rotation_vector_mapping(item);
        std::cout << "To go from"; print(vector); std::cout << "to "; print(item);
        std::cout << "\n"; print(matrix);
    }
}
