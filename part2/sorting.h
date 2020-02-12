#ifndef SORTING_H
#define SORITNG_H

struct RestDist {
  uint16_t index;
  uint16_t dist;
};

void iSort(RestDist array[], int length);
void qSort(RestDist array[], int low, int high);

#endif