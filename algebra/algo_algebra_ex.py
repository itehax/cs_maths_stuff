from math import prod

# si scriva un programma che dato x in N, verifichi se 2 | n, 3 | n, 4 | n, 5 | n, 7 | n, 9 | n, 11 | n, 2^k | n. Scopo didattico usare operatore mod builtin più efficente.
# funziona in base 10, x = 10^n*a_n+10^n-1*a_n-1+...+10^1*a_1+10^0*a_0, ritorna lista di coefficienti a_0,...,a_n-1,a_n in questo ordine.
# poco elegante ma funziona.


def get_base_coefficient(x: int) -> list:
    coefficients = []
    step = 10
    while True:
        fixup = step // 10
        coef = (x % step) // fixup
        coefficients.append(coef)
        x = x - (coef * fixup)
        if x == 0:
            break
        step *= 10
    return coefficients


# 10^3 = -1 mod 0, 10^0 = 1 mod 0, se 10^3k, 3k pari => 10^3k = 1 mod 7, se dispari => 10^3k = -1 mod 7.
# posso riscrivere x = 10^n*a_n+10^n-1*a_n-1+...+10^1*a_1+10^0*a_0 in questo modo: 10^0(a_0a_1a_2) + 10^3(a_3a_4a_5) + ... + 10^n-3(a_(n-2)a_(n-1)a_n)
# to fix
def is_div_by_7(x: int) -> bool:
    coefs = get_base_coefficient(x)
    # add missing zeros
    fixup = len(coefs) % 3
    if fixup != 0:
        for _ in range(3 - fixup):
            coefs.append(0)

    return (
        sum([pow(-1, i) * prod(coefs[i : i + 3]) for i in range(0, len(coefs), 3)]) % 7
        == 0
    )


def is_div_by_9(x: int) -> bool:
    return sum(get_base_coefficient(x)) % 9 == 0


# 10^n = 1 mod 11 if n=2k, otherwise 10=-1 mod 11 if n=2k+1
def is_div_by_11(x: int) -> bool:
    coefs = get_base_coefficient(x)
    return (
        sum([pow(-1, i) * c for c, i in zip(coefs, range(1, len(coefs) + 1))]) % 11 == 0
    )


# 10^n = 2^n * 5^n = 0 mod 10, inoltre 10^n = 0 mod 2^k per ogni n>=k => mi interessano solo ultime k cifre.
def is_div_by_2k(x: int, k: int) -> bool:
    i = 1
    res = 0
    for coef in get_base_coefficient(x)[:k]:
        res += coef * i
        i *= 10
    return res % pow(2, k) == 0


# voglio mostrare che x = 0 mod 2, in particolare so ckhe 10^n = 0 mod 2 per ogni n in n => basta verificare se a_0 = 0 mod 2
def is_div_by_2(x: int) -> bool:
    return get_base_coefficient(x)[0] % 2 == 0


def is_div_by_3(x: int) -> bool:
    return sum(get_base_coefficient(x)) % 3 == 0


def is_div_by_4(x: int) -> bool:
    coefficients = get_base_coefficient(x)
    s = coefficients[0] + coefficients[1] * 10
    return s % 4 == 0


# voglio mostrare che x = 0 mod 5, in particolare so che 10^n = 0 mod 5 per ogni n in n => basta verificare se a_0 = 0 mod 5
def is_div_by_5(x: int) -> bool:
    return get_base_coefficient(x)[0] % 5 == 0


if __name__ == "__main__":
    for i in range(0, 10000, 7):
        assert is_div_by_7(i) == True
