import numpy as np

class Board:
    def __init__(self, board_lines: [str]):
        assert len(board_lines) == 5
        self.board = np.zeros((5,5), dtype=int)
        for row in range(5):
            for col in range(5):
                self.board[row, col] = int(board_lines[row][col])
        self.marked = np.zeros((5,5), dtype=bool)

    def draw(self, number: int) -> bool:
        for row in range(5):
            for col in range(5):
                if self.board[row, col] == number:
                    self.marked[row, col] = True
                    return self.check_bingo(row, col)

    def check_bingo(self, row, col) -> bool:
        if all(self.marked[row]):
            return True
        if all(self.marked[:,col]):
            return True
        return False

    def sum_unmarked(self):
        return np.sum(self.board[~self.marked])

def read_input() -> ([int], [Board]):
    line = input()
    numbers = map(int, line.split(','))
    empty_line = input()
    assert empty_line == ""
    try_line = input()
    boards = []
    while len(try_line):
        board_lines = [try_line.split()]
        for i in range(4):
            line = input()
            board_lines.append(line.split())
        boards.append(Board(board_lines))
        try:
            empty_line = input()
        except EOFError:
            break
        else:
            assert empty_line == ""
            try_line = input()
    return numbers, boards

boards_finished = set()

def draw(number: int, boards: [Board]):
    for i, board in enumerate(boards):
        if board.draw(number):
            if not len(boards_finished):
                print('Part one:', board.sum_unmarked() * number)
            if i in boards_finished:
                continue
            else:
                boards_finished.add(i)
                if len(boards_finished) == len(boards):
                    print('Part two:', board.sum_unmarked() * number)
                    return

if __name__ == "__main__":
    numbers, boards = read_input()
    for number in numbers:
        r = draw(number, boards)
