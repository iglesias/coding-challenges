from collections import Counter
import sys

while True:
  line = sys.stdin.readline()
  if len(line)==0:
    break

  card, bid = line.split()
  #print(card, bid)
  c = Counter(card)
  h = sorted(list(c.values()))
  if h==[1,1,1,1,1]:
    print(card, 'high')
  elif h==[1,1,1,2]:
    print(card, 'pair')
  elif h==[1,2,2]:
    print(card, 'two pairs')
  elif h==[1,1,3]:
    print(card, 'three of a kind')
  elif h==[2,3]:
    print(card, 'full house')
  elif h==[1,4]:
    print(card, 'four of a kind')
  elif h==[5]:
    print(card, 'five of a kind')
  else:
    assert(False, 'Unexpected card frequencies.')
