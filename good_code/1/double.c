#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int linenum;
int getword(FILE *fp, char *buf, int size);
void doubleword(char *fname, FILE *fp);
void doubleword_fix(char *fname, FILE *fp);

int main(int argc, char *argv[]) {
  for (int i = 1; i < argc; ++i) {
    FILE *fp = fopen(argv[i], "r");
    if (fp == NULL) {
      fprintf(stderr, "%s: can't open '%s' (%s)\n", argv[0], argv[i],
              strerror(errno));
      return EXIT_FAILURE;
    } else {
      doubleword_fix(argv[i], fp);
      fclose(fp);
    }
  }
  if (argc == 1) {
    doubleword(NULL, stdin);
  }
  return EXIT_SUCCESS;
}

// reads word from a file, a word is one or more nonspaced chars.
// getword read the next word from an opened file into buf[0..size-1].
// return 0 if EOF is reached, otherwise return 1.
int getword(FILE *fp, char *buf, int size) {
  int c = getc(fp);
  // scan forward to a nonspace character or EOF
  for (; c != EOF && isspace(c); c = getc(fp)) {
    if (c == '\n')
      ++linenum;
  }
  // copy word into buf[0..size-1]
  int i = 0;
  for (; c != EOF && !isspace(c); c = getc(fp)) {
    if (i < size - 1) {
      buf[i++] = tolower(c);
    }
  }
  // useless in this case, but always useful.
  if (i < size) {
    buf[i] = '\0';
  }
  // if c isspace, leave it here for next word.
  if (c != EOF) {
    ungetc(c, fp);
  }
  return buf[0] != '\0';
}

void doubleword(char *fname, FILE *fp) {
  char prev[128], word[128];
  linenum = 1;
  prev[0] = '\0';

  while (getword(fp, word, sizeof(word))) {
    if (isalpha(word[0]) && strcmp(prev, word) == 0) {
      if (fname) {
        printf("%s:", fname);
      }
      printf("%d: %s\n", linenum, word);
    }
    strcpy(prev, word);
  }
}

// What does double print when it sees three or more identical
// words in its input? Change double to fix this “feature.”
void doubleword_fix(char *fname, FILE *fp) {
  char prev[128], word[128];
  char repeated[128];

  linenum = 1;
  prev[0] = '\0';
  repeated[0] = '\0';

  while (getword(fp, word, sizeof(word))) {
    if (isalpha(word[0]) && strcmp(prev, word) == 0) {
      if (strcmp(repeated, word) == 0) {
        continue;
      } else {
        if (fname) {
          printf("%s:", fname);
        }
        printf("%d: %s\n", linenum, word);
        strcpy(repeated, word);
      }
    }
    strcpy(prev, word);
  }
}
