#include <iostream>
#include <algorithm>
using namespace std;

int pivot(int a[], int n, int pi) {
  swap(a[pi], a[n-1]);
  int lo = 0, hi = n - 2;
  while (hi >= lo) {
    if (a[lo] <= a[n-1]) {
      lo++;
    } else if (a[hi] > a[n-1]) {
      hi--;
    } else {
      swap(a[lo], a[hi]);
    }
  }
  swap(a[lo], a[n-1]);
  return lo;
}

void qsort(int a[], int n) {
  if (n<= 1) {return;}
  int pi = n/2;
  int new_pi = pivot(a, n, pi);
  qsort(a, new_pi);
  qsort(a, n - new_pi - 1);
}

int main() {
  int n;
  cin >> n;
  int a[n];
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  qsort(a, n);
  for (int i = 0; i < n; i++) {
    cout << a[i] << ' ';
  }
  cout << endl;
  return 0;
}