# optimized egcd algorithm: dato au + bv = gcd(a,b), trova (u,v)
# assume a >= b
# dato a,b ritorna gcd(a,b), u,v
def egcd(a, b):
    # check if b = 0
    # add a way to return sol with u>0 small as possible
    if b > a:
        a, b = b, a
    if b == 0:
        return [a, 1, 0]
    u = 1
    g = a
    x = 0
    y = b
    while y != 0:
        q, r = divmod(g, y)
        s = u - q * x
        u = x
        g = y
        x = s
        y = r
    v = (g - a * u) // b
    # in case i want u > 0, i know that there are exactly g=gcd(a,b) unique sol in mod b, nella forma (x_0+ib_0,y_0-ia_0) per i<g,n in N
    # b = b_0*g, a = a_0*g, dunque div euclidea
    i = 0
    if u <= 0:
        u_0 = u // g
        v_0 = v // g
        while u <= 0 and i < g:
            u += i * v_0
            v -= i * u_0
            i = i + 1
    return [g, u, v]


if __name__ == "__main__":
    assert egcd(23, 0)[0] == 23 * egcd(23, 0)[1] + (0 * egcd(23, 0)[2])
    assert egcd(1258, 527)[0] == 1258 * egcd(1258, 527)[1] + 527 * egcd(1258, 527)[2]
    assert egcd(228, 1056)[0] == 1056 * egcd(228, 1056)[1] + 228 * egcd(228, 1056)[2]
    assert (
        egcd(163961, 167171)[0]
        == 167171 * egcd(163961, 167171)[1] + 163961 * egcd(163961, 167171)[2]
    )
