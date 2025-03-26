from z3 import *

MASK = 0xFFFFFFFF


def xorrathul64p(k1, k2):
    k1 = k1 & MASK
    k2 = k2 & MASK
    r = (k1 ^ k2) & MASK
    k1 = ((((k1 << 23) | k1 >> (32 - 23)) & MASK) ^ k2 ^ (k2 << 7)) & MASK
    k2 = ((k2 << 18) | k2 >> (32 - 18)) & MASK
    return r, k1, k2


def gen_rand(k1, k2):
    return xorrathul64p(k1, k2)[0] & 0x1FFFFFFF


def break_prng():
    k1, k2 = BitVecs("k1 k2", 32)
    known = [8440991, 9895740, 1648057]
    s = Solver()
    for rand in known:
        _, k_1, k_2 = xorrathul64p(k1, k2)
        s.add(gen_rand(k1, k2) == rand)
        k1, k2 = k_1, k_2
    if s.check() == sat:
        print("orkkk")
        m = s.model()
        print(m)


# ROUNDS = 3
# MASK = (1 << 64) - 1
#
##############################
##      SPECK functions      #
##############################
#
#
## Rotate 64 bit integer x right by r
# def ror(x, r):
#    return ((x >> r) | (x << (64 - r))) & MASK
#
#
## Rotate 64 bit integer x left by r
# def rol(x, r):
#    return ((x << r) | (x >> (64 - r))) & MASK
#
#
## SPECK round function; x, y, and k are all 64 bits
# def r(x, y, k):
#    x = ror(x, 8)
#    x += y
#    x &= MASK
#    x ^= k
#    y = rol(y, 3)
#    y ^= x
#    return x, y
#
#
## SPECK undo round function; x, y, and k are all 64 bits
# def unr(x, y, k):
#    y ^= x
#    y = ror(y, 3)
#    x ^= k
#    x -= y
#    x &= MASK
#    x = rol(x, 8)
#    return x, y
#
#
## SPECK encrypt function; all inputs are 64 bits
##   x || y is the plaintext
##   a || b is the key
# def encrypt(x, y, a, b):
#    for i in range(ROUNDS):
#        x, y = r(x, y, b)
#        a, b = r(a, b, i)
#    return x, y
#
#
## SPECK decrypt function; all inputs are 64 bits
##   x || y is the ciphertext
##   a || b is the key
# def decrypt(x, y, a, b):
#    for i in range(ROUNDS):
#        a, b = r(a, b, i)
#    for i in range(ROUNDS - 1, -1, -1):
#        a, b = unr(a, b, i)
#        x, y = unr(x, y, b)
#    return x, y
#
#
##############################
##     z3 SPECK functions    #
##############################
#
#
# def z3_ror(x, r):
#    return RotateRight(x, r)
#
#
# def z3_rol(x, r):
#    return RotateLeft(x, r)
#
#
## Write a z3 version of the SPECK round function, `r`
## HINTS:
##   * Remember to use the z3 versions of `rol` and `ror`
##     defined just above
##   * z3 BitVectors already deal with overflow, so you can
##     ignore the MASK operation
# def z3_r(x, y, k):
#    x = z3_ror(x, 8)
#    x += y
#    x ^= k
#    y = z3_rol(y, 3)
#    y ^= x
#    return x, y
#
#
## Write a z3 version of the SPECK encrypt function
## HINTS:
##   * Remember to use the z3 version of the round function you wrote
# def z3_encrypt(x, y, a, b):
#    for i in range(ROUNDS):
#        x, y = z3_r(x, y, b)
#        a, b = z3_r(a, b, i)
#    return x, y
#
#
## Given a plaintext, ciphertext pair encrypted using 3-Round SPECK,
## use z3 to derive the secret key, and then use that key to decrypt
## a secret message
# def break_speck():
#    plaintext = binascii.unhexlify("6c617669757165207469206564616d20")
#    ciphertext = binascii.unhexlify("8b7de2836dece7b9a5871dfecf9d0551")
#    p1 = struct.unpack(">Q", plaintext[:8])[0]
#    p2 = struct.unpack(">Q", plaintext[8:])[0]
#    c1 = struct.unpack(">Q", ciphertext[:8])[0]
#    c2 = struct.unpack(">Q", ciphertext[8:])[0]
#
#    x = BitVecVal(p1, 64)
#    y = BitVecVal(p2, 64)
#    a = BitVec("a", 64)
#    b = BitVec("b", 64)
#
#    try:
#        s = Solver()
#        o1, o2 = z3_encrypt(x, y, a, b)
#        s.add(o1 == c1)
#        s.add(o2 == c2)
#    except:
#        print("FAIL")
#        return
#
#    if s.check():
#        print("SUCCESS")
#        m = s.model()
#        k1 = m[a].as_long()
#        k2 = m[b].as_long()
#
#        real_ctext = binascii.unhexlify("7625ed6dd6ee6d2c58d5ae4f217d39da")
#        c1 = struct.unpack(">Q", real_ctext[:8])[0]
#        c2 = struct.unpack(">Q", real_ctext[8:])[0]
#        p1, p2 = decrypt(c1, c2, k1, k2)
#
#        plaintext = struct.pack(">Q", p1) + struct.pack(">Q", p2)
#        print(plaintext)
#
#    else:
#        print("FAIL")
#
#
# if __name__ == "__main__":
#    break_speck()
