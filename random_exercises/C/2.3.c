#include <ctype.h>
#include <stdio.h>
char hex_to_dec(char hex) {
  char dec;
  if (!isdigit(hex)) {
    // assume ABCDEF only uppercase, anyway easy
    return hex - 'A' + 10;
  }
  return hex - '0';
}
int htoi(char hex[]) {
  int n = 0;
  int i = 0;
  while (isdigit(hex[i]) || (hex[i] >= 'A' && hex[i] <= 'F')) {
    n = 16 * n + hex_to_dec(hex[i++]);
  }
  return n;
}

int main(void) {

  printf("%d %d\n", htoi("0"), 0x0);
  printf("%d %d\n", htoi("A"), 0xA);
  printf("%d %d\n", htoi("10"), 0x10);
  printf("%d %d\n", htoi("A1"), 0xA1);
  printf("%d %d\n", htoi("AB"), 0xAB);
  printf("%d %d\n", htoi("100"), 0x100);
  printf("%d %d\n", htoi("1A2"), 0x1A2);
  printf("%d %d\n", htoi("10A"), 0x10A);
  printf("%d %d\n", htoi("7FFFFFF"), 0x7FFFFFF);
  printf("%d %d\n", htoi("7FFFFFF1"), 0x7FFFFFF1);
  printf("%d %d\n", htoi("7FFFFFF2"), 0x7FFFFFF2);
  printf("%d %d\n", htoi("7FFFFFFE"), 0x7FFFFFFE);
  printf("%d %d\n", htoi("7FFFFFFF"), 0x7FFFFFFF);
  printf("%d %d\n", htoi("80000000"), 0x7FFFFFFF + 1);
  printf("%d %d\n", htoi("80000001"), 0x7FFFFFFF + 2);

  printf("%d %d\n", htoi("10AX"), 0x10A);
  printf("%d %d\n", htoi("203!"), 0x203);

  return 0;
}
