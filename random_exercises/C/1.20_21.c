#include <math.h>
#include <stdio.h>
/*
 Write a program detab that replaces tabs in the input with the proper number of
 blanks to space to the next tab stop. Assume a fixed set of tab stops, say
 every n columns. Should n be a variable or a symbolic parameter?
*/
#define TAB_STOP 4
#define BUF_SIZE 100
#define DETAB_BUF_SIZE ((BUF_SIZE - 1) * TAB_STOP + 1)

// assume buf well formed.
void detab(char buf[], char detab_buf[]) {
  int j = 0;
  for (int i = 0; buf[i] != '\0'; ++i) {
    if (buf[i] == '\t') {
      int n_spaces = ceil((double)j / TAB_STOP) * TAB_STOP - j;
      for (int k = 0; k < n_spaces; ++k) {
        detab_buf[j++] = ' ';
      }
    } else {
      detab_buf[j++] = buf[i];
    }
  }
  detab_buf[j] = '\0';
}
void intab(char buf[], char intab_buf[]) {
  int j = 0;
  int i = 0;
  while (buf[i] != '\0') {
    if (buf[i] == ' ') {
      int spaces = 0;
      while (buf[i] == ' ') {
        ++spaces;
        ++i;
      }
      int n_tabs = spaces / TAB_STOP;
      int n_spaces = spaces % TAB_STOP;
      for (int k = 0; k < n_tabs; ++k) {
        intab_buf[j++] = '\t';
      }
      for (int k = 0; k < n_spaces; ++k) {
        intab_buf[j++] = ' ';
      }
    } else {
      intab_buf[j++] = buf[i];
      ++i;
    }
  }
  intab_buf[j] = '\0';
}
// at most buf[len - 1] = '\0';
int getlinee(char buf[], int len) {
  int i;
  int c;
  for (i = 0; i < len - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
    buf[i] = c;
  }
  if (c == '\n') {
    buf[i] = c;
    ++i;
  }
  buf[i] = '\0';
  return i;
}

int main() {
  char buf[BUF_SIZE];
  printf("\ttest\n");
  char detab_buf[DETAB_BUF_SIZE];
  char intab_buf[BUF_SIZE];
  int l;
  while ((l = getlinee(buf, BUF_SIZE)) > 0) {
    detab(buf, detab_buf);
    printf("Detabbed: %s\n", detab_buf);
    intab(buf, intab_buf);
    printf("Intabbed: %s\n", intab_buf);
  }
  return 0;
}
