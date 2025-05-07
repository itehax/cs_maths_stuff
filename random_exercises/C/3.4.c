void trim(char s[]) {
  unsigned len = 0;
  while (s[len] != '\0') {
    ++len;
  }

  for (int i = len - 1; i >= 0; --i) {
    if (s[i] != ' ' && s[i] != '\t' && s[i] != '\n') {
      s[i + 1] = '\0';
      break;
    }
  }
}
