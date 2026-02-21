#include <array>
#include <bitset>
#include <cassert>
#include <functional>
#include <iostream>
#include <queue>
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

void rotation_vector_mapping(std::array<symbol, 3> const& vector, std::array<symbol, 3> const& item) {
    //TODO a goes to this position negated or not
    //     b goes to that ...
    //     c goes to ...
}

void generate_rotations() {
    // vector [a b c]^T is used as "canonical".
    std::array<symbol, 3> const vector = [](){
        std::array<symbol, 3> vector;
        vector.at(0).id = 0, vector.at(0).negative = false;
        vector.at(1).id = 1, vector.at(1).negative = false;
        vector.at(2).id = 2, vector.at(2).negative = false;
        return vector;
    }();

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
    for (auto const& item : Qed) {
        print(item);
        std::cout << '\n';
        rotation_vector_mapping(vector, item);
    }
    if (Qed.size() != 24) throw std::runtime_error("Unexpected #rotations.");
}

int main() {
    generate_rotations();
}
