#include <stdio.h>

#define SIZE 100
#define SIZE_ESCAPED SIZE * 2 // if all escape chars, SIZE*2 -1 max

void escape(char src[], char dst[]);
void unescape(char src[], char dst[]);

int getlinee(char buf[], int len);

int main() {
  char unescaped[SIZE];
  char escaped[SIZE_ESCAPED];
  while (getlinee(unescaped, SIZE) > 0) {
    escape(unescaped, escaped);
    puts(escaped);
    unescape(escaped, unescaped);
    puts(unescaped);
  }
}

void escape(char src[], char dst[]) {
  int k = 0;
  for (int i = 0; src[i] != '\0'; ++i) {
    switch (src[i]) {
    case '\n':
      dst[k++] = '\\';
      dst[k++] = 'n';
      break;
    case '\t':
      dst[k++] = '\\';
      dst[k++] = 't';
      break;
    default:
      dst[k++] = src[i];
      break;
    }
  }
  dst[k] = '\0';
}
void unescape(char src[], char dst[]) {
  int k = 0;
  int i;
  for (i = 0; src[i + 1] != '\0'; ++i) {
    if (src[i] == '\\') {
      switch (src[i + 1]) {
      case 'n':
        dst[k++] = '\n';
        ++i;
        break;
      case 't':
        dst[k++] = '\t';
        ++i;
        break;
      default:
        dst[k++] = '\\';
        break;
      }
    } else {
      dst[k++] = src[i];
    }
  }
  dst[k++] = src[i];
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
