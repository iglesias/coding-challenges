import numpy as np
import pyclipr
import sys

def dirchar2delta(dirchar):
    if dirchar == "R":      return complex(0, +1)
    elif dirchar == "D":    return complex(+1, 0)
    elif dirchar == "L":    return complex(0, -1)
    elif dirchar == "U":    return complex(-1, 0)
    else:                   assert(False)

def digitchar2dir(digitchar):
    if digitchar == "0":    return "R"
    elif digitchar == "1":  return "D"
    elif digitchar == "2":  return "L"
    elif digitchar == "3":  return "U"
    else:                   assert(False)

def read():
    dirchars = []
    lengths = []
    colors = []
    while True:
        line = sys.stdin.readline()
        if len(line) == 0: break
        dirchar, length, color = line.split()
        dirchars.append(dirchar)
        lengths.append(int(length))
        colors.append(color)
    return dirchars, lengths, colors

def convert(colors):
    dirchars = []
    lengths = []
    for color in colors:
        dirchars.append(digitchar2dir(color[-2]))
        lengths.append(int(color[2:-2], 16))
    return dirchars, lengths

def shape(dirchars, lengths):
    X = [complex(0,0)]
    for dirchar, length in zip(dirchars, lengths):
        X.append(X[-1])
        delta = dirchar2delta(dirchar)
        X[-1] += (length)*delta
    return X

def inflate(X):
    path = []
    for i in range(len(X)): path.append((X[i].imag, -X[i].real))
    po = pyclipr.ClipperOffset()
    po.scaleFactor = int(1000)
    po.addPaths([np.array(path)], pyclipr.JoinType.Miter, pyclipr.EndType.Polygon)
    po.addPaths([np.array(path)], pyclipr.JoinType.Miter, pyclipr.EndType.Polygon)
    offsetSquare = po.execute(1)
    return offsetSquare[0]

def area(x, y):
    return 0.5*np.abs(np.dot(x,np.roll(y,1))-np.dot(y,np.roll(x,1)))

def perimeter(x, y):
    p = 0
    for i in range(len(x)-1): p += abs(x[i]-x[i+1]) + abs(y[i]-y[i+1])
    return p

def methoda(X):
    x = np.imag(X)
    y = np.real(X)
    a = area(x,y)
    p = perimeter(x,y)
    print('  Method 1:', 'area =', a, ' perimeter =', p, ' answer =', a+p/2+1)

def methode(X):
    I = inflate(X)
    x = [*I[:,0]]
    y = [*I[:,1]]
    # Close the polygon given by pyclipr. FIXME nicer inside inflate.
    x.append(x[0])
    y.append(y[0])
    a = area(x,y)
    p = perimeter(x,y)
    print('  Method 2:', 'area =', a, ' perimeter =', p, ' answer =', a-p/2+1)

if __name__=='__main__':
    dirchars, lengths, colors = read()

    print('Part one:')
    X = shape(dirchars, lengths)
    methoda(X)
    methode(X)

    print('Part two:')
    colored_dirchars, colored_lengths = convert(colors)
    Y = shape(colored_dirchars, colored_lengths)
    methoda(Y)
    methode(Y)
