import matplotlib.pyplot as plt
import networkx as nx

nodes = set()
edges = []

for line in open(0):
  head, rest = line.split('->')
  head = head.strip();
  rest = rest.strip()
  rest = [item.strip() for item in rest.split(',')]
  if head != 'broadcaster': head = head[1:]
  nodes.add(head)
  for item in rest:
    nodes.add(item)
    edges.append((head, item))

G = nx.DiGraph()
G.add_nodes_from(list(sorted(nodes)))
G.add_edges_from(edges)

pos = nx.spring_layout(G, seed=135711)

options = {"with_labels": True}
nx.draw_networkx(G, pos=pos, **options)

plt.show()
