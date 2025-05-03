/*
Consider sorting n numbers stored in array A[1..n] by first finding the smallest
element of A[1..n] and exchanging it with the element in A[1]. Then find the
smallest element of A[2..n], and exchange it with A[2]. Then find the smallest
element of A[3..n], and exchange it with A[3]. Continue in this manner for the
first n-1 elements of A. Write pseudocode for this algorithm, which is known as
selection sort. What loop invariant does this algorithm maintain? Why does it
need to run for only the first n-1 elements, rather than for all n elements?
Give the worst-case running time of selection sort in Theta-notation. Is the
best-case running time any better?
*/
#include <iostream>
int find_min_index(int *arr, int start, int end) {
  int min = arr[start];
  int index = start;
  for (int i = start + 1; i < end; ++i) {
    if (arr[i] < min) {
      min = arr[i];
      index = i;
    }
  }
  return index;
}
void swap(int &x, int &y) {
  int tmp = x;
  x = y;
  y = tmp;
}
// ho array[0:n-1]. invariante di ciclo prima di iterazione i, array[0:i-1] è
// ordinato init: nel momento in cui i = 0; array[0:0] è ordinato, per def.
// mantenimento: per hp(per fatto che ad ogni iterazione viene trovato il min e
// messo alla prima posizione e cosi via) array da [0:i-1] sarà ordinato,dopo
// esecuzione di corpo del for, alla posizione a[i] verrà inserito elem minimo
// innrange [i:n-1] dunque alla prossima iter l'invariante verrà mantenuta.
// tenminazione: termina quando i = n - 1=> a[i:n-2] sortato, inoltre per come
// funziona algo,ultimo elemento sarà max. cvd.
// THETA(N^2)
void selection_sort(int *arr, int n) {
  for (int i = 0; i < n - 1; ++i) {
    int min_index = find_min_index(arr, i, n);
    swap(arr[i], arr[min_index]);
  }
}
