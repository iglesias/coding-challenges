# The Identity function.
def I(a):
    return a


# The Self-Applicative function.
def M(f):
    return f(f)
  # M(M) ¬ β-normal form


def K(a, b=None):
    return (lambda _: a) if b is None else a

Konstant = K('onstant')
# print(Konstant('what?'))

assert K(I)('x')('y') == 'y'

def KI(a, b=None):
    return (lambda _: b) if a is None else b

assert(K(M,KI) == K(M,K(I)) == M)
assert(KI(M,K) == K(I)(M)(K) == K)
assert(KI(K,M) == K(I)(K)(M) == M)

