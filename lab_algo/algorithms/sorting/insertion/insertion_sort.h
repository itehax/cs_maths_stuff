template <typename T> void insertion_sort(T *arr, unsigned size) {
  for (int i = 1; i < size; ++i) {
    T key = arr[i];
    // Inserisce key nel sottoarray arr[0:i-1] ordinato(invariante)
    int j = i - 1;
    while (j >= 0 && key < arr[j]) {
      arr[j + 1] = arr[j];
      --j;
    }
    arr[j + 1] = key;
  }
}
