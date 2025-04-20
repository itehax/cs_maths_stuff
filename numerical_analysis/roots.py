# Implementa il metodo di bisezione utilizzato per trovare radici di eq non lineare.
# Criterio di arresto, e_k = |x_k - sol| <= eps <=> |a_k - b_k| <= eps

# for fun, so che eps = più piccolo x: x+1 > 1
def get_eps():
    eps = 1.0
    while eps + 1.0 > 1:
        eps /= 2
    eps *= 2
    return eps


eps = get_eps()


# assunzione, f continua in [a;b] e f(a)f(b)<0
def bisect(f, a, b):
    while True:
        m = (a + b) / 2
        if abs(a - b) < eps:
            return m
        if f(a) * f(m) < 0:
            b = m
        else:
            a = m


# potrei scegliere come x0 anche valore prefissato per "garantire" convergenza
def secant(f, a, b):
    der = (f(b) - f(a)) / (b - a)  # coeff angolare
    # retta secante passante per due punti a,b è y = ((f(b) - f(a)) / (b - a))(x - a) + f(a)
    # voglio y = 0 => x = -f(a) / der + a
    # continuo ad aprossimare x fino a criterio di arresto.
    x_k = a
    x_k_n = -f(a) / der + a
    while f(x_k_n) < eps or abs(x_k_n - x_k) < eps:
        x_k = x_k_n
        x_k_n = -f(x_k) / der + x_k
    return x_k_n


if __name__ == "__main__":
    f = lambda x: x**2 - x - 1
    approx_phi = bisect(f, 1, 2)
    print(approx_phi)
    p = lambda x: x**3 - x**2 - 1
    approx = secant(p, 1, 2)
    print(approx)
