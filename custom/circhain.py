#!/usr/bin/env python

num_vertices = 5000
num_links = 2*num_vertices

print num_vertices, num_links

for i in xrange(1, num_vertices):
  print i, i+1
  print i+1, i

print num_vertices, 1
print 1, num_vertices
