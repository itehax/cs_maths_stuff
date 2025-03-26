# Implementa il metodo di bisezione utilizzato per trovare radici di eq non lineare.
# Criterio di arresto, e_k = |x_k - sol| <= eps <=> |a_k - b_k| <= eps

# for fun, so che eps = più piccolo x: x+1 > 1
def get_eps():
    eps = 1.0
    while eps + 1.0 > 1:
        eps /= 2
    eps *= 2
    return eps


# assunzione, f continua in [a;b]
def bisect(f, a, b):
    if f(a) * f(b) < 0:
        eps = get_eps()
        a_k = a
        b_k = b
        while abs(a_k - b_k) > eps:
            m = (a_k + b_k) / 2
            if f(m) == 0:
                return m
            elif f(a_k) * f(b_k) < 0:
                a_k = m
            else:
                b_k = m

        return f((a_k+b_k)/2)


def foo(x):
    return x**2 - 4


if __name__ == "__main__":
    f = lambda x: x**2 - x - 1
    approx_phi = bisect(f, 1, 2)
    print(approx_phi)
