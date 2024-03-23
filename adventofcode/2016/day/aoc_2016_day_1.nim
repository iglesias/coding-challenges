import strutils

# import system/iterators and use mitems instead of having to created strippedToken?

type
  Direction = tuple[x, y: int]

const
  DIRECTIONS: array[4, Direction] = [(0, 1), (1, 0), (0, -1), (-1, 0)]

proc solve(line: string): int =
    var
        x = 0
        y = 0
        h = 0
    for token in line.split(','):
        let strippedToken = token.strip()
        let letter = strippedToken[0]
        let digits = strippedToken[1..high(strippedToken)].parseInt()
        h = (((h + (if letter == 'R': 1 else: -1)) mod 4) + 4) mod 4
        x += DIRECTIONS[h][0] * digits
        y += DIRECTIONS[h][1] * digits
    return abs(x) + abs(y)

echo solve(readLine(stdin).strip())
