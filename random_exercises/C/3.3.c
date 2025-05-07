/*
EX 3.3: Write a function expand(s1,s2) that expands shorthand notations like
a-z in the string s1 into the equivalent complete list abc...xyz in s2 Allow
for letters of either case and digits, and be prepared to handle cases like
a-b-c and a-z0-9 and -a-z

case supported:
a-z
-a-z
a-z0-9
a-b-c...
*/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
void expand(char src[], char dst[]);
int getlinee(char buf[], int len);
int main() {
  char buf[128];
  char expanded[512];
  while (getlinee(buf, 128) > 0) {
    expand(buf, expanded);
    puts(expanded);
  }
  return EXIT_SUCCESS;
}

void expand(char src[], char dst[]) {
  int k = 0;
  for (int i = 0; src[i] != '\0'; ++i) {
    // skip spaces and -
    for (; isspace(src[i]) || src[i] == '-'; ++i)
      ;
    for (char start = src[i]; start <= src[i + 2] && src[i + 1] == '-';
         ++start) {
      dst[k++] = start;
    }
    i += 2;
  }
  dst[k] = '\0';
}

int getlinee(char buf[], int len) {
  int i;
  int c;
  for (i = 0; i < len - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
    buf[i] = c;
  }
  if (c == '\n') {
    buf[i++] = c;
  }
  buf[i] = '\0';
  return i;
}
