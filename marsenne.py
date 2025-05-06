BIT_MASK_32 = 0xffffffff
STATE_VECTOR_LEN = 624
STATE_VECTOR_M = 397
UPPER_MASK = 0x80000000
LOWER_MASK = 0x7fffffff
TEMPERING_MASK_B = 0x9d2c5680
TEMPERING_MASK_C = 0xefc60000

class MT:
    def __init__(self,seed:int) -> None:
        self.state_array = []
        self.state_index = 0
        self.init_state(seed)

    def init_state(self,seed):
        self.state_array.append(seed & BIT_MASK_32)
        for i in range(1,STATE_VECTOR_LEN):
            seed = (1812433253 * (seed ^ (seed >> (32 -2))) + i) & BIT_MASK_32
            self.state_array.append(seed)

    def gen_rand_uint32(self):
        k = self.state_index
        #point to state STATE_VECTOR_LEN - 1 iter before
        j = k - (STATE_VECTOR_LEN - 1) % STATE_VECTOR_LEN
        #modulo, for circular indexing, only needed for first STATE_VECTOR_LEN iter
        #if j < 0:
        #    j+=STATE_VECTOR_LEN

        #made by msb of s[k] and first 31 lsb of s[j]
        x = (self.state_array[k] & UPPER_MASK | self.state_array[j] & LOWER_MASK) & BIT_MASK_32

        xA = x >> 1
        if x & 0x1:
            xA^=0x9908b0df
        #same idea as previuous j
        j = k - (STATE_VECTOR_LEN-STATE_VECTOR_M) % STATE_VECTOR_LEN

        #compute next value in the state
        x = self.state_array[j] ^ xA
        self.state_array[k] =x
        k+=1

        self.state_index = k % STATE_VECTOR_LEN

        #tampering
        y = (x ^ (x >> 11)) & BIT_MASK_32
        y = (y ^ ((y << 7) & TEMPERING_MASK_B)) & BIT_MASK_32
        y = (y ^((y << 15) & TEMPERING_MASK_C)) & BIT_MASK_32
        return (y ^ (y >> 18)) & BIT_MASK_32


a = MT(0x1337)

for i in range(624):
    print(a.gen_rand_uint32())
