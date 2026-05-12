#include <cassert>
#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <numeric>
#include <optional>
#include <sstream>
//#include <stdexcept>
#include <thread>
#include <csignal>

void read_presents();
bool solve();

constexpr int P = 3;

constexpr int num_present_shapes = 6;
char all_configurations[num_present_shapes][8][P][P];

void print_present(char p[P][P]) {
  std::cout << "\n";
  for (int i = 0 ; i < P; i++) {
    for (int j = 0; j < P; j++) std::cout << p[i][j];
    std::cout << '\n';
  }
}

// first row -> last (third) column
// second row -> second column
// third row -> first column
void rotate(char p[P][P], char rotated[P][P]) {
  for (int i = 0 ; i < P; i++) for (int j = 0; j < P; j++)
    rotated[j][P-i-1] = p[i][j];
}

void flip(char p[P][P], char flipped[P][P]) {
  for (int i = 0 ; i < P; i++) for (int j = 0; j < P; j++)
    flipped[i][P-j-1] = p[i][j];
}

class Solver {
public:
std::atomic<bool> interrupt{false};

int width;
int length;
std::vector<std::vector<char>> region;
std::array<int, num_present_shapes> presents_left;
char current_present_char = 'A';
bool solve() {
  if (interrupt.load(std::memory_order_relaxed)) {
    return false; // abort
  }
  /*
  std::cout << '\n';
  PRINT(presents_left);
  for (int l = 0 ; l < length; l++) {
    for (int w = 0; w < width; w++) std::cout << region[l][w];
    std::cout << '\n';
  }
  std::cout << '\n';
  */
  for (int const pl : presents_left) assert(pl >= 0);
  if (std::accumulate(presents_left.begin(), presents_left.end(), 0) == 0) {
    std::cout << '\n';
    for (std::vector<char> const& vc : region) {
      for (char const c : vc) std::cout << c;
      std::cout << '\n';
    }
    std::cout << '\n';
    return true;
  }
  int p = 0;
  for (int const pl : presents_left) {
    if (pl != 0) break;
    p++;
  }
  for (int l = 0 ; l < length - P + 1; l++) for (int w = 0; w < width - P + 1; w++) {
    if (interrupt.load(std::memory_order_relaxed)) {
      return false; // abort
    }
    if (region[l][w] != '.') continue;
//    for every "rotation and flip" of the next _present_
    for (int r = 0; r < 8; r++) {
//      if the _present_ (possibly rotated and/or flipped) fits at this *cell*
      if (interrupt.load(std::memory_order_relaxed)) {
        return false; // abort
      }
      bool occupied = false;
      for (int i = 0 ; i < P && !occupied; i++) for (int j = 0; j < P && !occupied; j++) {
        if (all_configurations[p][r][i][j] == '#' && region.at(l + i).at(w + j) != '.') {
          occupied = true;
          break;
        }
      }
      if (interrupt.load(std::memory_order_relaxed)) {
        return false; // abort
      }
      if (occupied) continue;
      if (interrupt.load(std::memory_order_relaxed)) {
        return false; // abort
      }
//        paint the grid inserting the present at this *cell*
      for (int i = 0 ; i < P; i++) for (int j = 0; j < P; j++) {
        if (all_configurations[p][r][i][j] != '#') continue;
        assert(region[l + i][w + j] == '.');
        //region[l + i][w + j] = '#';
        region[l + i][w + j] = current_present_char;
      }
//        reduce the present count to account for the insertion
      assert(presents_left[p] > 0);
      if (current_present_char == 'Z') current_present_char = 'A';
      else current_present_char++;
      assert('A' <= current_present_char && current_present_char <= 'Z');
      presents_left[p]--;
//        recurse with the updated grid and presents count
      if (solve()) return true;
//        unpaint grid
      if (current_present_char == 'A') current_present_char = 'Z';
      else current_present_char--;
      assert('A' <= current_present_char && current_present_char <= 'Z');
      for (int i = 0 ; i < P; i++) for (int j = 0; j < P; j++) {
        if (all_configurations[p][r][i][j] != '#') continue;
        assert(region[l + i][w + j] == current_present_char);
        region[l + i][w + j] = '.';
      }
//        undo reduction of count
      assert(presents_left[p] >= 0);
      presents_left[p]++;
    }  // for (int r = 0; r < 8; r++)
  }  // for (int l = 0 ; l < length - P - 1; l++) for (int w = 0; w < width - P - 1; w++)
  return false;
}
};

std::optional<bool> try_solve_with_timeout(Solver& solver, int timeout = 10) {
  solver.interrupt.store(false);
  auto future = std::async(std::launch::async, &Solver::solve, &solver);
  if (future.wait_for(std::chrono::milliseconds(timeout)) == std::future_status::timeout) {
    solver.interrupt.store(true);
    // WAIT until the thread actually finishes!
    // Block until unwinding is complete.
    future.wait();
    return std::nullopt;
  }
  return future.get();
}

auto const start = std::chrono::high_resolution_clock::now();

int problem_idx = 0;
std::array<Solver, 1000> solvers;
void signal_handler(int signal)
{
  if (signal == SIGINT) solvers[problem_idx].interrupt = true;
}

int main() {
  std::signal(SIGINT, signal_handler);
  read_presents();
  std::cout << "presents read!" << std::endl;
  for (int shape_idx = 0; shape_idx < num_present_shapes; shape_idx++) {
    //print_present(all_configurations[shape_idx][0]);
    int next_configuration = 1;
    char rotated[P][P];
    rotate(all_configurations[shape_idx][0], rotated);
    for (int i = 0 ; i < P; i++) for (int j = 0; j < P; j++)
      all_configurations[shape_idx][1][i][j] = rotated[i][j];
    next_configuration = 2;
    //print_present(rotated);
    for (int r = 0; r < 2; r++) {
      char rotatedn[P][P];
      rotate(rotated, rotatedn);
      for (int i = 0 ; i < P; i++) for (int j = 0; j < P; j++)
        all_configurations[shape_idx][next_configuration][i][j] = rotatedn[i][j];
      next_configuration++;
      //print_present(rotatedn);
      for (int i = 0 ; i < P; i++) for (int j = 0; j < P; j++)
        rotated[i][j] = rotatedn[i][j];
    }
    char flipped_and_then_rotated[P][P];
    flip(all_configurations[shape_idx][0], flipped_and_then_rotated);
    for (int i = 0 ; i < P; i++) for (int j = 0; j < P; j++)
      all_configurations[shape_idx][next_configuration][i][j] = flipped_and_then_rotated[i][j];
    next_configuration++;
    //print_present(flipped_and_then_rotated);
    for (int r = 0; r < 3; r++) {
      char flippedrotatedn[P][P];
      rotate(flipped_and_then_rotated, flippedrotatedn);
      for (int i = 0 ; i < P; i++) for (int j = 0; j < P; j++)
        all_configurations[shape_idx][next_configuration][i][j] = flippedrotatedn[i][j];
      next_configuration++;
      //print_present(flippedrotatedn);
      for (int i = 0 ; i < P; i++) for (int j = 0; j < P; j++)
        flipped_and_then_rotated[i][j] = flippedrotatedn[i][j];
    }
  }
  std::cout << "all_configurations filled!" << std::endl;
  std::string line;
  int ans = 0;
  std::vector<std::future<std::optional<bool>>> futures;
  while(std::getline(std::cin, line)) {
    Solver& solver = solvers.at(problem_idx);
    //width = 50;//12; //4;
    //length = 45;// 5; //4;
    solver.width = std::stoi(line.substr(0, line.find('x')));
    solver.length = std::stoi(line.substr(line.find('x')+1,line.find(':')-line.find('x')-1));
    solver.region = std::vector(solver.length, std::vector(solver.width, '.'));
    {
      int i = 0;
      std::istringstream iss(line.substr(line.find(' ') + 1));
      for (std::string token; iss >> token;)
        solver.presents_left.at(i++) = std::stoi(token);
    }
    //presents_left = std::array{0, 0, 0, 0, 2, 0};
    //presents_left = std::array{1, 0, 1, 0, 3, 2};
    //presents_left = std::array{40, 43, 39, 39, 40, 39};
    std::cout << "solve()...";
    futures.push_back(std::async(try_solve_with_timeout, std::ref(solver), 100000));
    futures.back().wait();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    problem_idx++;
  }
  /*
  for (auto& future : futures) {
    auto maybe_value = future.get();
    if (maybe_value.has_value()) ans += maybe_value.value();
  }
  */
  auto const end = std::chrono::high_resolution_clock::now();
  //std::cout << "Part one: " << ans << "\n";
  std::cout << std::chrono::duration<double, std::milli>(end - start).count() << " milliseconds\n";
  //if (ans != 528) throw std::runtime_error("wrong answer");
}

void read_presents() {
  for (int i = 0; i < num_present_shapes; i++) {
    std::string line;
    std::getline(std::cin, line);
    std::stringstream ss(line);
    int j;
    char colon;
    ss >> j >> colon;
    assert(j == i);
    assert(colon == ':');
    std::getline(std::cin, line);
    for (int k = 0; k < P; k++) all_configurations[i][0][0][k] = line[k];
    std::getline(std::cin, line);
    for (int k = 0; k < P; k++) all_configurations[i][0][1][k] = line[k];
    std::getline(std::cin, line);
    for (int k = 0; k < P; k++) all_configurations[i][0][2][k] = line[k];
    std::getline(std::cin, line);
    assert(line.empty());
  }
}
