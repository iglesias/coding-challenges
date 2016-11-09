#include <array>
#include <cassert>
#include <cstring>
#include <cctype>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

enum Color {
  WHITE = 0,
  BLACK,
};

char PieceAndColorToChar(char piece, Color color) {
  switch(color) {
  case WHITE:
    return toupper(piece);
  case BLACK:
    return tolower(piece);
  }

  assert(false);
  return '\0';
}

Color CharToColor(char c) {
  if (isupper(c)) return WHITE;
  else            return BLACK;
}

class Board {
public:
 Board();
 Board(const Board& other);

 void SetPiece(int row, int col, char piece, Color color);
 void SetFree(int row, int col);
 bool IsOccupiedColor(int row, int col, Color color) const;
 void Print() const;

 bool CanWin(Color color, vector<Board>* next_boards) const;
 bool CanMovesWin(int row, int col, Color color, vector<Board>* next_boards) const;
 bool CanKnightMovesWin(int row, int col, Color color, vector<Board>* next_boards) const;

public:
 static const int kSize = 4;
 static const char kFree = ' ';

private:
// void CheckInBounds(int row, int col) const;
 bool IsFree(int row, int col) const;
 bool CanPieceWin(int row, int col, vector<Board>* next_boards) const;

private:
 array<array<char, kSize>, kSize> board_;
};

Board::Board() {
  for (int i = 0; i < kSize; i++)
    for (int j = 0; j < kSize; j++)
      board_[i][j] = kFree;
}

Board::Board(const Board& other) {
  for (int i = 0; i < kSize; i++)
    memcpy(board_[i].data(), other.board_[i].data(), kSize);
//    for (int j = 0; j < kSize; j++)
//      board_[i][j] = other.board_[i][j];
}

void Board::SetPiece(int row, int col, char piece, Color color) {
//  CheckInBounds(row, col);
//  board_[row][col] = PieceAndColorToChar(piece, color);
  board_.at(row).at(col) = PieceAndColorToChar(piece, color);
}

void Board::SetFree(int row, int col) {
//  CheckInBounds(row, col);
//  board_[row][col] = kFree;
  board_.at(row).at(col) = kFree;
}

bool Board::IsOccupiedColor(int row, int col, Color color) const {
//  CheckInBounds(row, col);
//  return board_[row][col] != kFree && CharToColor(board_[row][col] == color); 
  return !IsFree(row, col) && CharToColor(board_[row][col]) == color; 
}

void Board::Print() const {
  for (int row = kSize-1; row >= 0; row--) {
    for (int col = 0; col < kSize; col++)
      cout << board_[row][col];
    cout << endl;
  }
}

bool Board::CanWin(Color color, vector<Board>* next_boards) const {
  for (int row = 0; row < kSize; row++) {
    for (int col = 0; col < kSize; col++) {
      if (IsOccupiedColor(row, col, color) &&
          CanPieceWin(row, col, next_boards)) return true;
    }
  }

  return false;
}

bool Board::CanMovesWin(int row, int col, Color color,
                        vector<Board>* next_boards) const {
  static const unordered_map<char, vector<array<int, 2>>> moves = {
    // Queen.
    {'q', {{0,1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {1,-1,}, {-1,1}, {-1,-1}}},
    // Bishop.
    {'b', {{1,1}, {1,-1,}, {-1,1}, {-1,-1}}},
    // Rook.
    {'r', {{0,1}, {0,-1}, {1,0}, {-1,0}}},
  };

  const auto& current_moves = moves.at(tolower(board_.at(row).at(col)));
  for (const array<int, 2>& move : current_moves) {
    int r = row+move[0], c = col+move[1];
    while (0 <= r && r < kSize && 0 <= c && c < kSize) {
      if (IsFree(r, c)) {
        Board new_board(*this);
        new_board.SetPiece(r, c, board_.at(row).at(col), color);
        new_board.SetFree(row, col);
        next_boards->push_back(new_board);
      } else if (IsOccupiedColor(r, c, color)) {
        break;
      } else {
        // Piece of the other color where we are moving.
        char piece = board_.at(r).at(c);
        assert(CharToColor(piece) != color);
        if (tolower(piece) ==  'q') return true;
        Board new_board(*this);
        new_board.SetPiece(r, c, board_.at(row).at(col), color);
        new_board.SetFree(row, col);
        next_boards->push_back(new_board);
        break;
      }
    
      r += move[0];
      c += move[1];
    }
  }

  return false;
}

bool Board::CanKnightMovesWin(int row, int col, Color color,
                              vector<Board>* next_boards) const {
  static const vector<array<int, 2>> knight_moves = {{1,2}, {2,1}, {1,-2},
                                                     {2,-1}, {-1,2}, {-2,1},
                                                     {-1,-2}, {-2,-1}};

  for (const array<int, 2>& move : knight_moves) {
    int r = row+move[0], c = col+move[1];
    if (r < 0 || r >= kSize || c < 0 || c >= kSize) continue;

    if (IsFree(r, c)) {
      Board new_board(*this);
      new_board.SetPiece(r, c, board_.at(row).at(col), color);
      new_board.SetFree(row, col);
      next_boards->push_back(new_board);
    } else if (!IsOccupiedColor(r, c, color)) {
      // Piece of the other color where we are moving.
      char piece = board_.at(r).at(c);
//        cout << board_.at(row).at(col) << " moving to eat " << piece << endl;
      assert(CharToColor(piece) != color);
      if (tolower(piece) ==  'q') return true;
      Board new_board(*this);
      new_board.SetPiece(r, c, board_.at(row).at(col), color);
      new_board.SetFree(row, col);
      next_boards->push_back(new_board);
    }
  }

  return false;
}

bool Board::CanPieceWin(int row, int col, vector<Board>* next_boards) const {
  assert(!IsFree(row, col));
  
  char piece = board_.at(row).at(col);
  Color color = CharToColor(piece);

  switch (piece) {
  // Queen.
  case 'Q':
  case 'q':
  // Bishop.
  case 'B':
  case 'b':
  // Rook.
  case 'R':
  case 'r':
    return CanMovesWin(row, col, color, next_boards);
  // Knight.
  case 'N':
  case 'n':
    return CanKnightMovesWin(row, col, color, next_boards);
  }

  assert(false);
}

bool Board::IsFree(int row, int col) const {
  return board_.at(row).at(col) == kFree;
}

/*
void Board::CheckInBounds(int row, int col) const {
  assert(0 <= row && row < kSize);
  assert(0 <= col && col < kSize);
}
*/

// IsWinNode must be called when BLACK made the last move.
bool IsWinNode(const Board& board, int move_depth, int depth_max) {
  // Ensure that black made the last move.
  assert(move_depth%2 == 0);

  // If white has already made all her moves, this is not a win node.
  if (move_depth >= depth_max) return false;

  vector<Board> next_boards;
  bool can_win = board.CanWin(WHITE, &next_boards);

  // If black's queen can be directly taken by white, this is a win node.
  if (can_win) return true;

  // If the last move is for black and white cannot directly take black's queen
  // in the before-to-last move, this is not a win node.
  if (move_depth+1 == depth_max) return false;

  // Recursive case. This is a win node if at least one next_boards will lead
  // to next_next_boards that are all win nodes.
  for (const Board& next_board : next_boards) {
    vector<Board> next_next_boards; // An awful name.
    if (next_board.CanWin(BLACK, &next_next_boards)) continue;
    int i = 0;
    while (i < next_next_boards.size() &&
           IsWinNode(next_next_boards[i], move_depth+2, depth_max)) i++;
    if (i == next_next_boards.size()) return true;
  }

  return false;
}

int main() {
  int g;
  cin >> g;

  for (int gi = 0; gi < g; gi++) {
    int w, b, m;
    cin >> w >> b >> m;
    Board board;

    for (int wi = 0; wi < w; wi++) {
      char piece, col;
      int row;
      cin >> piece >> col >> row;
      board.SetPiece(row-1, col-'A', piece, WHITE);
    }

    for (int bi = 0; bi < b; bi++) {
      char piece, col;
      int row;
      cin >> piece >> col >> row;
      board.SetPiece(row-1, col-'A', piece, BLACK);
    }

    cout << (IsWinNode(board, 0, m) ? "YES" : "NO") << endl;
  }

  return 0;
}
