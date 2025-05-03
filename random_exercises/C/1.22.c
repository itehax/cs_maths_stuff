#include <stdio.h>
/*
Write a program to ``fold'' long input lines into two or more shorter lines
after the last non-blank character that occurs before the n-th column of input.
Make sure your program does something intelligent with very long lines, and if
there are no blanks or tabs before the specified column.
*/

#define MAX_LINE_LENGTH 5
#define BUF_SIZE 128
// at most buf[len - 1] = '\0';
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
//to finish
void fold(char buf[], char folded_buf[], int buf_len) {
  int n_full_chunks = buf_len / MAX_LINE_LENGTH;
  int remaining = buf_len % MAX_LINE_LENGTH;
  int folded_i = 0;

  for (int i = 0; i < n_full_chunks; ++i) {
    int found_word = 0;
    for (int j = 0; j < MAX_LINE_LENGTH; ++j) {
      char cur = buf[i * MAX_LINE_LENGTH + j];
      if ((cur != ' ' && cur != '\t') || found_word == 0) {
        if (cur != ' ' && cur != '\t') {
          found_word = 1;
        }
        folded_buf[folded_i++] = cur;
      } else {
        folded_buf[folded_i++] = '\n';
        found_word = 0;
      }
    }
    if (found_word == 1) {
      folded_buf[folded_i++] = '\n';
    }
  }
  for (int i = 0; i < remaining; ++i) {
    // buf[MAX_LINE_LENGTH * n_full_chunks + i]
  }
  folded_buf[folded_i] = '\0';
}
// void fold(char buf[], char folded_buf[], int buf_len) {
//   int n_full_chunks = buf_len / MAX_LINE_LENGTH;
//   int remaining = buf_len % MAX_LINE_LENGTH;
//   int index_folded = 0;
//   for (int i = 0; i < buf_len; ++i) {
//     // find first word
//     int start = i;
//     // find word of len<= MAX_LINE_L. here buf[i..start] is word
//     while ((start - i) < MAX_LINE_LENGTH && buf[start] != ' ' &&
//            buf[start] != '\t') {
//       folded_buf[index_folded++] = buf[start++];
//     };
//     // now MAX_LINE_L - (START - I) - 1 white or other chars need to be
//     // inserted. + \n also need to convert tab to spaces.
//   }
// }
int main() {
  char buf[BUF_SIZE];
  char folded_buf[BUF_SIZE * 2]; // for simplicity, assume this max size.
  int buf_len;                   // buf[buf_len] = '\0'
  while ((buf_len = getlinee(buf, BUF_SIZE)) > 0) {
    fold(buf, folded_buf, buf_len);
    printf("Folded=%s\n", folded_buf);
  }
}
