#include "arith.h"
int Arith_max(int x, int y) { return (x > y) ? x : y; }
int Arith_min(int x, int y) { return !(x > y) ? x : y; }
int Arith_div(int x, int y) {
  // behave like floor
  return x / y - (x % y != 0 && (x ^ y) < 0); // diff sign
}
int Arith_mod(int x, int y) {
  // derived easy from definition of euclidean division
  return x - y * Arith_div(x, y);
}
extern int Arith_ceiling(int x, int y) {
  return Arith_floor(x, y) + (x % y != 0);
}
int Arith_floor(int x, int y) { return Arith_div(x, y); }
