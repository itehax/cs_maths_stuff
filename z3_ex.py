from z3 import *

# class XoRRayhulRNG {
#  constructor(s0, s1) {
#    this.s0 = s0;
#    this.s1 = s1;
#  }
#
#  xorrayhul64p(){
#    var s0 = (this.s0>>>0) & MASK;
#    var s1 = (this.s1>>>0) & MASK;
#
#    var r = (s0 + s1)>>>0 & MASK;
#
#    s1 = ((s0 ^ s1)>>>0) & MASK;
#    this.s0 = ((((s0 << 23)>>>0 | s0 >>> (32 - 23)) & MASK) ^ s1 ^ (s1 << 7)>>>0) & MASK;
#    this.s1 = ((s1 << 18)>>>0 | s1 >>> (32 - 18)) & MASK;
#    return r;
#  }
#
#  gen_rand(){
#    var r = this.xorrayhul64p() & 0x1FFFFFF;
#    console.log("r:", r);
#    var index = r % 37;
#    var text = options[index];
#    return text;
#  }
# }
MASK = 0xFFFFFFFF  # 2^32 - 1 = 32bit set a 1, dunque forzo 4 byte


class PRNG:
    def __init__(self, state0, state1):
        self.s0 = state0
        self.s1 = state1

    def xorrayhul64p(self):
        s0 = self.s0 & MASK
        s1 = self.s1 & MASK
        random = (s0 + s1) & MASK
        s1 = (s0 ^ s1) & MASK
        self.s0 = ((((s0 << 23) | s0 >> (32 - 23)) & MASK) ^ s1 ^ (s1 << 7)) & MASK
        self.s1 = ((s1 << 18) | s1 >> (32 - 18)) & MASK
        return random

    def gen_rand(self):
        return (self.xorrayhul64p() & 0x1FFFFFF) % 37


def z3_xorarrayhul64p(s, state0, state1, expected):
    # dont care about masking cause 32 bit already enforced in z3, need to take care only about      logical shift, because >> is arithmetic, so sign preserved.
    random = (state0 + state1) & 0x1FFFFFF
    s.add(expected == random)
    state1 = state0 ^ state1
    state0 = ((state0 << 23) | LShR(state0, (32 - 23))) ^ state1 ^ (state1 << 7)
    state1 = (state1 << 18) | LShR(state1, (32 - 18))
    return state0, state1


def break_prng():
    state0, state1 = BitVecs("state0 state1", 32)
    stolen = [25342117, 28906081, 13411728]
    curr_state0, curr_state_1 = state0, state1
    s = Solver()
    # this works because curr_state_i depends on state0 and state1.
    for r in stolen:
        curr_state0, curr_state_1 = z3_xorarrayhul64p(s, curr_state0, curr_state_1, r)

    if s.check() == sat:
        m = s.model()
        seed0 = m[state0].as_long()
        seed1 = m[state1].as_long()
        print(f"BROKEN! got {m[state0]} as state0, {m[state1]} as state1")
        rng = PRNG(seed0, seed1)
        for _ in range(10):
            print(f"Next gen value:{rng.gen_rand()}")


if __name__ == "__main__":
    break_prng()
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
