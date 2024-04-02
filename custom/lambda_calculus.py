# The Identity function.
def I(a):
    return a


# The Self-Applicative function.
def M(f):
    return f(f)
  # M(M) ¬ β-normal form


def K(a, b=None):
    return (lambda _: a) if b is None else a

assert K(I)('x')('y') == 'y'
