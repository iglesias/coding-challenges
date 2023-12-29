import sets

let input = readLine(stdin)

type pair = tuple[r, c: int]

func dir2delta(c: char): pair =
  case c
  of '>': return (0,+1)
  of 'v': return (+1,0)
  of '<': return (0,-1)
  of '^': return (-1,0)
  else:   assert(false)

func `+`(a: pair, b: pair): pair = (a.r+b.r, a.c+b.c)

iterator move(input: string, first_location: pair): pair =
  var
    i = 0
    location = first_location
  while i < len(input):
    location = location + dir2delta(input[i])
    yield location
    inc(i)

var visited_houses = initHashSet[pair]()
visited_houses.incl((0,0))
for location in move(input, (0,0)):
  visited_houses.incl(location)

echo "Part one: ", len(visited_houses), '.'
