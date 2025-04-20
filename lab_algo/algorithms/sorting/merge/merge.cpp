#include <iostream>
template <typename T>
void merge(T *arr, int start, int mid, int end, T *left, T *right) {
  int len_left = mid - start + 1;
  int len_right = end - mid;
  for (int i = 0; i < len_left; ++i) {
    left[i] = arr[start + i];
  }
  for (int i = 0; i < len_right; ++i) {
    right[i] = arr[mid + 1 + i];
  }
  int i = 0;
  int j = 0;
  int curr = start;
  while (i < len_left && j < len_right) {
    if (left[i] <= right[j]) {
      arr[curr] = left[i++];
    } else {
      arr[curr] = right[j++];
    }
    ++curr;
  }
  while (i < len_left) {
    arr[curr] = left[i++];
    ++curr;
  }
  while (j < len_right) {
    arr[curr] = right[j++];
    ++curr;
  }
}
template <typename T>
void merge_sort(T *arr, int start, int end, T *left, T *right) {
  if (start >= end) {
    return;
  }
  T mid = (start + end) / 2;
  merge_sort(arr, start, mid, left, right);
  merge_sort(arr, mid + 1, end, left, right);
  merge(arr, start, mid, end, left, right);
}
