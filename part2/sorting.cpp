#include "coordinates.h"
#include "sorting.h"

// swaps the values of two RestDist variables
void swapRest(RestDist *restA, RestDist *restB) {
  RestDist tmp = *restA;
  *restA = *restB; *restB = tmp;
}

// implementation of insertion sort from assignment pdf
void iSort(RestDist array[], int length) {
  int i = 1;
  while (i < length) {
    int j = i;
    while (j > 0 && array[j-1].dist > array[j].dist) {
      swapRest(&array[j], &array[j-1]); j--;
    }
    i++;
  }
}

int pivot(RestDist a[], int length, int pi) {
  swapRest(&a[pi], &a[length - 1]);
  int lo = 0, hi = length - 2;
  while (hi >= lo) {
    if (a[lo].dist <= a[length - 1].dist) {
      lo++;
    } else if (a[hi].dist > a[length - 1].dist) {
      hi--;
    } else {
      swapRest(&a[lo], &a[hi]);
    }
  }
  swapRest(&a[lo], &a[length - 1]);
  return lo;
}

void qSort(RestDist a[], int length) {
  if (length<= 1) {return;}
  int pi = length/2;
  int new_pi = pivot(a, length, pi);
  qSort(a, new_pi);
  qSort(a, length - new_pi - 1);
}