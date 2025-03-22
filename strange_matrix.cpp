#include <iostream>

int main() {
  int mat[3][3];
  int n = 3;
  mat[0][0] = n - 1 + 1;
  for (int i = 1; i < n; ++i) {
    mat[i][0] = i - 1 + 1;
  }
  // for (int i = 0; i < 3; ++i) {
  //   std::cout << mat[i][0] << std::endl;
  // }
  for (int j = 1; j < n; ++j) {
    mat[0][j] = mat[n - 1][j - 1]+1;
    int ctr = 1;
    for (int i = 1; i < n; ++i) {
      mat[i][j] = mat[ctr++][j - 1]+1;
    }
  }
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cout << mat[i][j] << std::endl;
    }
  }
}
