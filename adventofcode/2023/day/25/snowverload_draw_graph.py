import sys
import networkx as nx
import matplotlib.pyplot as plt

G = nx.Graph()

while True:
    line = sys.stdin.readline()
    if len(line) == 0: break
    head, tail = line.split(':')
    for nodeStr in tail.split():
        G.add_edge(head, nodeStr)

nx.draw_networkx(G, with_labels=True)

plt.show()
