import binascii
import struct

from z3 import *

ROUNDS = 3
FORCE_64_BIT = (1 << 64) - 1

#############################
#      SPECK functions      #
#############################


# Rotate 64 bit integer x right by r
def ror(x, r):
    return ((x >> r) | (x << (64 - r))) & FORCE_64_BIT


# Rotate 64 bit integer x left by r
def rol(x, r):
    return ((x << r) | (x >> (64 - r))) & FORCE_64_BIT


# SPECK round function; x, y, and k are all 64 bits
def r(x, y, key):
    x = ror(x, 8)
    x += y
    x &= FORCE_64_BIT
    x ^= key
    y = rol(y, 3)
    y ^= x
    return x, y


# SPECK undo round function; x, y, and k are all 64 bits
def unr(x, y, key):
    y ^= x
    y = ror(y, 3)
    x ^= key
    x -= y
    x &= FORCE_64_BIT
    x = rol(x, 8)
    return x, y


# SPECK encrypt function; all inputs are 64 bits
#   x || y is the plaintext
#   a || b is the key
def encrypt(p_1, p_2, key_1, key_2):
    for i in range(ROUNDS):
        p_1, p_2 = r(p_1, p_2, key_2)
        key_1, key_2 = r(key_1, key_2, i)
    return p_1, p_2


# SPECK decrypt function; all inputs are 64 bits
#   x || y is the ciphertext
#   a || b is the key
def decrypt(x, y, a, b):
    for i in range(ROUNDS):
        a, b = r(a, b, i)
    for i in range(ROUNDS - 1, -1, -1):
        a, b = unr(a, b, i)
        x, y = unr(x, y, b)
    return x, y


#############################
#     z3 SPECK functions    #
#############################


def z3_ror(x, r):
    return RotateRight(x, r)


def z3_rol(x, r):
    return RotateLeft(x, r)


# Write a z3 version of the SPECK round function, `r`
# HINTS:
#   * Remember to use the z3 versions of `rol` and `ror`
#     defined just above
#   * z3 BitVectors already deal with overflow, so you can
#     ignore the FORCE_64_BIT operation
def z3_r(x, y, key):
    y ^= x
    y = z3_ror(y, 3)
    x ^= key
    x -= y
    x = z3_rol(x, 8)
    return x, y


# Write a z3 version of the SPECK encrypt function
# HINTS:
#   * Remember to use the z3 version of the round function you wrote
def z3_encrypt(p_1, p_2, key_1, key_2):
    for i in range(ROUNDS):
        p_1, p_2 = r(p_1, p_2, key_2)
        key_1, key_2 = r(key_1, key_2, i)
    return p_1, p_2


# Given a plaintext, ciphertext pair encrypted using 3-Round SPECK,
# use z3 to derive the secret key, and then use that key to decrypt
# a secret message
def break_speck():
    plaintext = binascii.unhexlify("6c617669757165207469206564616d20")
    ciphertext = binascii.unhexlify("8b7de2836dece7b9a5871dfecf9d0551")
    p1 = struct.unpack(">Q", plaintext[:8])[0]
    p2 = struct.unpack(">Q", plaintext[8:])[0]
    c1 = struct.unpack(">Q", ciphertext[:8])[0]
    c2 = struct.unpack(">Q", ciphertext[8:])[0]

    x = BitVecVal(p1, 64)
    y = BitVecVal(p2, 64)
    a = BitVec("a", 64)
    b = BitVec("b", 64)

    try:
        s = Solver()
        # basically i want to find a,b such that o1==c1 ...
        o1, o2 = z3_encrypt(x, y, a, b)
        s.add(o1 == c1)
        s.add(o2 == c2)
    except:
        print("FAIL")
        return

    if s.check():
        print("SUCCESS")
        m = s.model()
        k1 = m[a].as_long()
        k2 = m[b].as_long()

        real_ctext = binascii.unhexlify("7625ed6dd6ee6d2c58d5ae4f217d39da")
        c1 = struct.unpack(">Q", real_ctext[:8])[0]
        c2 = struct.unpack(">Q", real_ctext[8:])[0]
        p1, p2 = decrypt(c1, c2, k1, k2)

        plaintext = struct.pack(">Q", p1) + struct.pack(">Q", p2)
        print(plaintext)

    else:
        print("FAIL")


if __name__ == "__main__":
    break_speck()


def sodoku_solver():
    sodoku = [
        (5, 3, 0, 0, 7, 0, 0, 0, 0),
        (6, 0, 0, 1, 9, 5, 0, 0, 0),
        (0, 9, 8, 0, 0, 0, 0, 6, 0),
        (8, 0, 0, 0, 6, 0, 0, 0, 3),
        (4, 0, 0, 8, 0, 3, 0, 0, 1),
        (
            7,
            0,
            0,
            0,
            2,
            0,
            0,
            0,
            6,
        ),
        (0, 6, 0, 0, 0, 0, 2, 8, 0),
        (0, 0, 0, 4, 1, 9, 0, 0, 5),
        (0, 0, 0, 0, 8, 0, 0, 7, 9),
    ]

    matrix = [[Int(f"x_{i+1}__{j+1}") for i in range(9)] for j in range(9)]
    cells_c = [
        And(matrix[i][j] >= 1, matrix[i][j] <= 9) for i in range(9) for j in range(9)
    ]
    rows_c = [Distinct(matrix[i]) for i in range(9)]
    cols_c = [Distinct(matrix[i][j]) for i in range(9) for j in range(9)]
    sub_mat_c = [
        Distinct(
            [
                matrix[3 * i0 + disp_i][3 * j0 + disp_j]
                for disp_i in range(3)
                for disp_j in range(3)
            ]
        )
        for i0 in range(3)
        for j0 in range(3)
    ]
    soduku_c = cells_c + rows_c + cols_c + sub_mat_c
    instance_sod = [
        If(sodoku[i][j] == 0, True, matrix[i][j] == sodoku[i][j])
        for i in range(9)
        for j in range(9)
    ]
    s = Solver()
    s.add(soduku_c + instance_sod)
    if s.check() == sat:
        print(s.model())
