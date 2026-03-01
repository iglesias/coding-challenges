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

namespace math {

constexpr int dim = 3;
template<typename T> using vector = std::array<T, dim>;
using matrix = std::array<vector<int>, dim>;

};

struct transformation {
    math::matrix rotation;
    math::vector<int> translation;
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

void print(math::vector<symbol> const& a) {
    for (auto const& s : a) {
        print(s);
        std::cout << ' ';
    }
}

void print(math::matrix A) {
    for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++)
        std::cout << std::setw(2) << A.at(i).at(j) << (j == 2 ? "\n" : " ");
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

// symbolic vector [a b c]^T
std::array<symbol, 3> const v = [](){
        std::array<symbol, 3> v;
        v.at(0).id = 0, v.at(0).negative = false;
        v.at(1).id = 1, v.at(1).negative = false;
        v.at(2).id = 2, v.at(2).negative = false;
        return v;
    }();

auto generate_rotations() {
    std::unordered_set<std::array<symbol, 3>, boost::hash<std::array<symbol, 3>>> Qed;
    Qed.insert(v);
    std::queue<std::array<symbol, 3>> Q;
    Q.push(v);
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

    math::matrix rotation;
    rotation.fill(std::array<int, 3>{0, 0, 0});
    for (int offset = 0; offset < 3; offset++) {
        int const pos = find_symbol_position(item, 'a'+offset);
        rotation.at(offset).at(pos) = item.at(pos).negative ? -1 : 1;
    }
    return rotation;
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

auto operator*(math::matrix const& A, math::vector<int> const& v) {
    auto matmul = [A, v](int r){
        int c = 0;
        for (int k = 0; k < 3; k++) c += A.at(r).at(k) * v.at(k);
        return c;
    };
    return std::make_tuple(matmul(0), matmul(1), matmul(2));
}

int main() {
    auto const rotations = generate_rotations();

    auto const scans = read_input();

    std::unordered_map<int, std::unordered_map<int, transformation>> A;
    std::unordered_map<int, std::vector<int>> neighbors;

    for (size_t scan_idx_i = 0; scan_idx_i < scans.size(); scan_idx_i++) {
    for (size_t scan_idx_j = scan_idx_i + 1; scan_idx_j < scans.size(); scan_idx_j++) {

    auto const& scan0 = scans.at(scan_idx_i);
    auto const& scan1 = scans.at(scan_idx_j);

    bool overlap_found = false;
    for (size_t i = 0; i < scan0.size() && !overlap_found; i++) for (size_t j = 0; j < scan1.size() && !overlap_found; j++) {
        for (auto const& item : rotations) {
            auto const rotation = rotation_vector_mapping(item);
            // Apply rotation to scan1[j]
            auto const [x1, y1, z1] = rotation * scan1.at(j);
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
                auto const [xjj, yjj, zjj] = rotation * scan1.at(jj);
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
                A[scan_idx_i][scan_idx_j] = {rotation, {-dx, -dy, -dz}};
                neighbors[scan_idx_i].push_back(scan_idx_j);
                neighbors[scan_idx_j].push_back(scan_idx_i);
                std::println("{} -> {}:", scan_idx_i, scan_idx_j);
                if (scan_idx_i == 0 && scan_idx_j == 1) {
                    /*
                    std::println("Beacons relative to scanner 0:");
                    for (auto const& pq : matches) std::println("  {}", pq.first);
                    std::println("Beacons relative to scanner 1:");
                    for (auto const& pq : matches) std::println("  {}", pq.second);
                    */
                    std::println("Scanner 1 is at {}", A[0][1].translation);
                }
                break;
            }
        }
    }

    }
    }

    // Is there more than one connected component?
    /*
    {
        std::unordered_set<int> c;
        std::queue<int> d;
        c.insert(0);
        d.push(0);
        while (!d.empty()) {
            int const n = d.front();
            d.pop();
            for (int const m : neighbors[n]) if (!c.contains(m)) {
                c.insert(m);
                d.push(m);
            }
        }
        std::println("scans size: {}, #CC: {}", scans.size(), c.size());
    }
    std::println();
    */

    // Paths to scan0:
    {
        for (size_t scan_idx = 1; scan_idx < scans.size(); scan_idx++) {
            std::queue<std::pair</* node state */ int, /* state of the path to scan0 */ std::vector<int>>> Q;
            Q.emplace(scan_idx, std::vector<int>{});
            std::unordered_set<int> Qed;
            Qed.insert(scan_idx);
            bool scan0_found = false;
            while (!Q.empty()) {
                auto [node, path] = Q.front();
                Q.pop();
                for (int const neighbor : neighbors[node]) if (!Qed.contains(neighbor)) {
                    if (neighbor == 0) {
                        scan0_found = true;
                        std::print("{} -> ", scan_idx);
                        for (int const intermediate_node : path) {
                            std::print("{} -> ", intermediate_node);
                        }
                        std::println("0");

                        // Validating on sample input:
                        {
                            math::vector<int> p{0,0,0};
                            int from = scan_idx;
                            for (size_t path_idx = 0; path_idx <= path.size(); path_idx++) {
                                int const to = path_idx == path.size() ? 0 : path[path_idx];
                                std::print("  From {} to {}: ", from, to);
                                if (A.contains(from) && A.at(from).contains(to)) {
                                    std::println("direct");
                                    // translate
                                    for (int c = 0; c < 3; c++) {
                                        p[c] -= A[from][to].translation[c];
                                    }
                                    math::matrix inverse;
                                    for (int r = 0; r < 3; r++) for (int c = 0; c < 3; c++) {
                                        inverse[r][c] = A[from][to].rotation[c][r];
                                    }
                                    // rotate
                                    auto const [x, y, z] = A[from][to].rotation * p;
                                    p[0] = x, p[1] = y, p[2] = z;
                                } else {
                                    assert(A.contains(to) && A[to].contains(from));
                                    std::println("inverse");
                                    // rotate
                                    auto const [x, y, z] = A[to][from].rotation * p;
                                    p[0] = x, p[1] = y, p[2] = z;
                                    // translate
                                    for (int c = 0; c < 3; c++) {
                                        p[c] += A[to][from].translation[c];
                                    }
                                }
                                from = to;
                            }
                            std::println("Scanner {} is at {}", scan_idx, p);
                        }

                        break;
                    } else {
                        path.push_back(neighbor);
                        Qed.insert(neighbor);
                        Q.emplace(neighbor, path);
                    }
                }
            }
        }
    }
}
}
