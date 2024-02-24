#include <iostream>

template <typename T>
void merge_sort_recursive(T arr[], T reg[], int start, int end) {
  // 退出条件，只有一个元素时，不需要处理，直接退出
  if (start >= end) return;

  int len  = end - start, mid = (len >> 1) + start; 
  int start1 = start, end1 = mid;
  int start2 = mid + 1, end2 = end;
  // 先分治后归并
  merge_sort_recursive(arr, reg, start1, end1);
  merge_sort_recursive(arr, reg, start2, end2);
  int k = start;
  while (start1 <= end1 && start2 <= end2)
    reg[k++] = arr[start1] < arr[start2] ? arr[start1++] : arr[start2++];

  while (start1 <= end1)
    reg[k++] = arr[start1++];

  while (start2 <= end2)
    reg[k++] = arr[start2++];

  for (k = start; k <= end; k++)
    arr[k] = reg[k];
}

template <typename T>
void merge_sort(T arr[], const int len) {
  T* reg = new T[len];
  merge_sort_recursive(arr, reg, 0, len-1);
}

int main() {
  int arr[] = {14, 12, 15, 13, 11, 16};
  merge_sort(arr, 6);
  std::copy(arr, arr + 6, std::ostream_iterator<int>(std::cout, " "));
}