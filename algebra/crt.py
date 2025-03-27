# teorema cinese del resto, si aspetta una lista di resti e moduli: moduli coprimi tra loro <=> gcd(m_i,m_j) = 1 per ogni i!=j
from math import prod
from sage.all import *
def crt(remainders: list, mods: list):
    assert len(remainders) == len(mods)
    sol = 0
    final_mod = prod(mods)
    for i in range(len(mods)):
        r_i = mods[i]
        R_i = final_mod // r_i
        g,a,_ = xgcd(R_i,r_i)
        x_i = a #first coef of diophantine eq ax-by=gcd(a,b)
        sol += x_i*(remainders[i] //g)*R_i
    return sol % final_mod


if __name__ == "__main__":
    print(crt([2, 3, 5], [5, 11, 17]))
