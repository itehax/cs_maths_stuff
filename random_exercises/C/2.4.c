#include <stdio.h>
/*
An alternate version of squeeze(s1, s2) that deletes each character in
s1 that matches any character in the string s2
*/
enum Matches {
  FOUND,
  NOT_FOUND,
};
// this version do useless comparison, can create a cache of prev removed or
// just remove in s2.
void squeeze(char s1[], char s2[]) {
  int j;
  for (int i = j = 0; s1[i] != '\0'; ++i) {
    int found = NOT_FOUND;
    for (int k = 0; s2[k] != '\0'; ++k) {
      if (s1[i] == s2[k]) {
        found = FOUND;
        break;
      }
    }
    if (found == NOT_FOUND) {
      s1[j++] = s1[i];
    }
  }
  s1[j] = '\0';
}
int main() {
  char s2[] = "AZ";
  char s1[] = "AABCDZZEDDDZZZDAAADDSSD";
  squeeze(s1, s2);
  puts(s1);
}
