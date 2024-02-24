#include <algorithm>
#include <iostream>
#include <iterator>

template< typename T>
void merge_sort(T arr[], int len) {
  int* a = arr;
  int* b = new int[len * sizeof(int)];
  int seg, start;
  // 归并的步长, 1 2 4 8...
  for (seg = 1; seg < len; seg += seg) {
    for (start = 0; start < len; start += seg + seg) {
      // 开始时步长为1先[0, 1], [2, 3], [3, 4]..., 
      // 步长为2, [0, 1, 2], [3, 4, 5]...
      // 步长为4，[0, 1, 2, 3], [4, 5]...
      int low = start;
      int mid = std::min(start + seg, len);
      int high = std::min(start + seg + seg, len);

      int k = low;
      int start1 = low, end1 = mid;
      int start2 = mid, end2 = high;

      while (start1 < end1 && start2 < end2)
        b[k++] = a[start1] < a[start2] ? a[start1++] : a[start2++];
      
      while (start1 < end1) 
        b[k++] = a[start1++];

      while (start2 < end2)
        b[k++] = a[start2++];
    }
    // 将已经部分拍好序的部分给a，
    int* temp = a; 
    a = b;
    b = temp;
  }
  // 这里有点问题，好像a一定不会是arr, 
  if (a != arr) {
    int i = 0;
    for(; i < len; i++)
      b[i] = a[i];
    b = a;
  }
  delete[] b;
}

int main() {
  // int arr[] = {14, 12, 15, 13, 11, 16};
  // merge_sort<int>(arr, 6);
  // std::copy(arr, arr + 6, std::ostream_iterator<int>(std::cout, " "));

  int arr1[] = {1, 2, 3, 4};
  merge_sort<int>(arr1, 4);
}