# Algoritmo di esponenziazione modulare ottimo
# Input: g^A (mod n)
# Output: risultato di esponenziazione modulare in O(2log_2(A)) moltiplicazioni

# Dato A = A_0 + A1*b + A2*b^2 + ... + An*b^n
# ritorna coefficienti A_0,A_1,...,A_n in tale ordine.
from math import prod


def numberToBase(A, b):
    if A == 0:
        return [0]
    digits = []
    while A:
        digits.append(int(A % b))
        A //= b
    return digits


# pesante a livello di memoria si può fare di meglio
#Exponentiation by squaring.
def fast_powering(g, A, n):
    digits = numberToBase(A, 2)
    computed_factor = [g % n]
    for i in range(1, len(digits)):
        computed_factor.append((computed_factor[i - 1] ** 2) % n)

    return prod([(a**A) % n for (a, A) in zip(computed_factor, digits)]) % n


if __name__ == "__main__":
    # 3^218 mod 1000 = 489
    assert fast_powering(3, 218, 1000) == 489
