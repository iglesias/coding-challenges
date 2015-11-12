def flood_fill(i, j, n, island, checked, island_count):
  if checked[i][j]: return
  checked[i][j] = True
  if island[i][j] is 0: return
  island[i][j] = island_count

  for row in island:
    print row

  print
  for row in checked:
    print row

  print '\n'

  if i > 0:   flood_fill(i-1, j, n, island, checked, island_count)
  if i < n-1: flood_fill(i+1, j, n, island, checked, island_count)
  if j > 0:   flood_fill(i, j-1, n, island, checked, island_count)
  if j < n-1: flood_fill(i, j+1, n, island, checked, island_count)

island = [[0,1,0,1,0],[0,0,1,1,1],[1,0,0,1,0],[0,1,1,0,0],[1,0,1,0,1]]

n = len(island)

checked = []
for i in xrange(n):
  checked.append([])
  for j in xrange(n):
    checked[i].append(False)

island_count = 0 

for i in xrange(n):
  for j in xrange(n):
    if island[i][j] is 1 and not checked[i][j]:
      island_count += 1
      flood_fill(i, j, n, island, checked, island_count)

print island_count
