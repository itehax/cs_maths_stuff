#include "arith.h"
#include <stdio.h>

int main() {
  for (int i = -5; i <= 5; ++i) {
    printf("%d mod %d is %d\n", i, 5, Arith_mod(i, 5));
  }
}
