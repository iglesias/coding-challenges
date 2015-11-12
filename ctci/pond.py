def flood_fill(i, j, n, land, checked):
  if i < 0 or i > n-1 or j < 0 or j > n-1:
    return 0

  if checked[i][j]:
    return 0

  checked[i][j] = True

  if land[i][j] is not 0:
    return 0

  pond_size = 1;

  for di in xrange(-1,2):
    for dj in xrange(-1,2):
      pond_size += flood_fill(i+di, j+dj, n, land, checked)

  return pond_size

def main():
  land = [  [0,2,1,0],
            [0,1,0,1],
            [1,1,0,1],
            [0,1,0,1]   ]

  n = len(land)

  checked = []
  for i in xrange(n):
    checked.append([])
    for j in xrange(n):
      checked[i].append(False)

  for i in xrange(n):
    for j in xrange(n):
      if land[i][j] is 0 and not checked[i][j]:
        print flood_fill(i, j, n, land, checked),

if __name__ == "__main__":
  main()
