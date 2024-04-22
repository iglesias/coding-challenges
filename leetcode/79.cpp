// Word Search

#include <cassert>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using std::make_pair;
using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;

bool exist(vector<vector<char>> const &board, string word, int i, int r, int c,
           set<pair<int, int>> &taken) {
  if (i == static_cast<int>(word.size()))
    return true;

  vector<vector<int>> const deltas = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  int const m = static_cast<int>(board.size());
  int const n = static_cast<int>(board[0].size());
  for (auto const &delta : deltas) {
    int const rr = r + delta[0], cc = c + delta[1];
    if (!(rr >= 0 and rr < m and cc >= 0 and cc < n))
      continue;
    if (board[rr][cc] == word[i] and !taken.contains(make_pair(rr, cc))) {
      taken.emplace(rr, cc);
      if (exist(board, word, i + 1, rr, cc, taken))
        return true;
      taken.erase(make_pair(rr, cc));
    }
  }
  return false;
}

bool existByFreq(vector<vector<char>> const &board, string word) {
  map<char, int> wordCounter;
  for (char letter : word) {
    if (wordCounter.contains(letter)) {
      wordCounter[letter]++;
    } else {
      wordCounter.emplace(letter, 1);
    }
  }

  map<char, int> boardCounter;
  for (int r = 0; r < static_cast<int>(board.size()); r++)
    for (int c = 0; c < static_cast<int>(board[0].size()); c++)
      if (boardCounter.contains(board[r][c]))
        boardCounter[board[r][c]]++;
      else
        boardCounter.emplace(board[r][c], 1);

  for (auto const &[c, count] : wordCounter)
    if (!boardCounter.contains(c) or boardCounter[c] < count)
      return false;

  return true;
}

bool exist(vector<vector<char>> &board, string word) {
  if (!existByFreq(board, word))
    return false;

  for (int r = 0; r < static_cast<int>(board.size()); r++)
    for (int c = 0; c < static_cast<int>(board[0].size()); c++) {
      set<pair<int, int>> taken{{r, c}};
      if (board[r][c] == word[0] and exist(board, word, 1, r, c, taken))
        return true;
    }
  return false;
}

int main() {
  vector<vector<char>> board = {
      {'A', 'B', 'C', 'E'}, {'S', 'F', 'C', 'S'}, {'A', 'D', 'E', 'E'}};
  string word = "ABCCED";
  assert(exist(board, word));

  word = "SEE";
  assert(exist(board, word));

  word = "ABCB";
  assert(!exist(board, word));
}
