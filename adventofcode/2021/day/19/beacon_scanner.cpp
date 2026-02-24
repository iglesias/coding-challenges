#include <array>
#include <bitset>
#include <cassert>
#include <flat_set>
#include <functional>
#include <iomanip>
#include <iostream>
#include <print>
#include <queue>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>

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

struct transformation {
    std::array<std::array<int, 3>, 3> rotation;
    std::array<int, 3> translation;
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

auto read_scan_report(std::istream& input_stream) {
    std::string line;
    std::vector<std::array<int, 3>> scan_report;
    for (;;) {
        std::getline(input_stream, line);
        if (line.empty()) break;
        auto const p = line.find(',');
        int const x = std::stoi(line.substr(0, p));
        auto const q = line.find(',', p+1);
        int const y = std::stoi(line.substr(p+1, q-p));
        int const z = std::stoi(line.substr(q+1));
        scan_report.push_back({x, y, z});
    }
    return scan_report;
}

auto read_input() {
    std::vector<std::vector<std::array<int, 3>>> scans;
    std::string line;
    while (std::getline(std::cin, line)) {
        //assert(line.substr(0, 12) == "--- scanner ");
        scans.push_back(read_scan_report(std::cin));
    }
    return scans;
}

int const mininum_overlapping_beacons = 12;

int main() {
    auto const rotations = generate_rotations();

    auto const scans = read_input();

    std::unordered_map<int, std::unordered_map<int, transformation>> A;
    std::unordered_map<int, std::vector<int>> B;

    for (size_t scan_idx_i = 0; scan_idx_i < scans.size(); scan_idx_i++) {
    for (size_t scan_idx_j = scan_idx_i + 1; scan_idx_j < scans.size(); scan_idx_j++) {

    auto const& scan0 = scans.at(scan_idx_i);
    auto const& scan1 = scans.at(scan_idx_j);

    bool overlap_found = false;
    for (size_t i = 0; i < scan0.size() && !overlap_found; i++) for (size_t j = 0; j < scan1.size() && !overlap_found; j++) {
        for (auto const& item : rotations) {
            auto const matrix = rotation_vector_mapping(item);
            // Apply matrix to scan1[j]
            auto matmul = [&](int r, size_t idx1){
                int c = 0;
                for (int k = 0; k < 3; k++) c += matrix.at(r).at(k) * scan1.at(idx1).at(k);
                return c;
            };
            auto const x1 = matmul(0, j);
            auto const y1 = matmul(1, j);
            auto const z1 = matmul(2, j);
            // Compute shift from scan1[j] and scan0[i]
            auto const dx = x1 - scan0.at(i).at(0);
            auto const dy = y1 - scan0.at(i).at(1);
            auto const dz = z1 - scan0.at(i).at(2);
            // Assertion:
            assert(scan0[i] == std::make_tuple(x1-dx, y1-dy, z1-dz));
            // Count the number of points in scan1 that after rotating and shifting 
            // are points in scan0
            std::flat_set<std::tuple<int, int, int>> scan0set;
            for (size_t ii = 0; ii < scan0.size(); ii++) {
                if (ii == i) continue;
                scan0set.insert(scan0.at(ii));
            }
            int matches_count = 1; // starting from 1 because scan1[j]'s been placed at scan0[i]
            std::vector<std::pair<std::tuple<int, int, int>, std::tuple<int, int, int>>> matches;
            matches.emplace_back(scan0.at(i), scan1.at(j));
            for (size_t jj = 0; jj < scan1.size(); jj++) {
                if (jj == j) continue;
                auto const xjj = matmul(0, jj);
                auto const yjj = matmul(1, jj);
                auto const zjj = matmul(2, jj);
                auto const p = std::make_tuple(xjj-dx, yjj-dy, zjj-dz);
                if (scan0set.contains(p)) {
                    matches_count += 1;
                    scan0set.erase(p);
                    matches.emplace_back(p, scan1.at(jj));
                }
                if (matches_count == mininum_overlapping_beacons) break;
            }
            if (matches_count == mininum_overlapping_beacons) {
                overlap_found = true;
                A[scan_idx_i][scan_idx_j] = {matrix, {dx, dy, dz}};
                B[scan_idx_i].push_back(scan_idx_j);
                B[scan_idx_j].push_back(scan_idx_i);
                std::println("{} -> {}:", scan_idx_i, scan_idx_j);
                std::println("{}", A[scan_idx_i][scan_idx_j].rotation);
                std::println("{}\n", A[scan_idx_i][scan_idx_j].translation);
                if (scan_idx_i == 0 && scan_idx_j == 1) {
                    std::println("Beacons relative to scanner 0:");
                    for (auto const& pq : matches) std::println("  {}", pq.first);
                    std::println("Beacons relative to scanner 1:");
                    for (auto const& pq : matches) std::println("  {}", pq.second);
                    std::println("Scanner 1 is at {}.", A[0][1].translation); // appears negated
                }
                break;
            }
        }
    }

    }
    }

    // Is there more than one connected component?
    {
        std::unordered_set<int> c;
        std::queue<int> d;
        c.insert(0);
        d.push(0);
        while (!d.empty()) {
            int const n = d.front();
            d.pop();
            for (int const m : B[n]) if (!c.contains(m)) {
                c.insert(m);
                d.push(m);
            }
        }
        std::println("scans size: {}, #CC: {}", scans.size(), c.size());
    }
}
}
