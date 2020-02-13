#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>

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

// pivots the array and returns a new pivot index new_pi
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

// recursive function to sort RestDist array
// implemented from pseudocode in eclass
void qSort(RestDist a[], int length) {
  if (length<= 1) {return;}
  int pi = length/2;
  int new_pi = pivot(a, length, pi);
  qSort(a, new_pi);
  qSort(a, length - new_pi - 1);
}

// converts restaurant longitude and latitude 
// and calculates the Manhattan distance from cursor 
int16_t calculateDist(restaurant rest) {
  int16_t restX = map(rest.lon, LON_WEST, LON_EAST, 0, YEG_SIZE);
  int16_t restY = map(rest.lat, LAT_NORTH, LAT_SOUTH, 0, YEG_SIZE); 
  int16_t cursorX_coord = cursorX + currentPatchX;
  int16_t cursorY_coord = cursorY + currentPatchY;
  return abs(restX - cursorX_coord) + abs(restY - cursorY_coord);
}

// clears RestDist array
void clearRDArray() {
  for (int i = 0; i < NUM_RESTAURANTS; i++) {
    rest_dist[i].index = 0;
    rest_dist[i].dist = 0;
  }
}

// filters the list of restaurants depending on rating
// and saves them into a RestDist array for 
int filterRestaurants() {
  int actualLen = 0; restaurant currentRest;
  for (int i = 0; i < NUM_RESTAURANTS; i++) {
    getRestaurantFast(i, &currentRest);
    if ((currentRest.rating+1)/2 >= ratingSel - 1) {
      rest_dist[i].index = i;
      rest_dist[i].dist = calculateDist(currentRest);
      actualLen++;
    }
  }
  return actualLen;
}

// runs sorting test and prints out running time on serial
// returns the length of the filtered array
int runSort(sort setting) {
  clearRDArray();
  int len = filterRestaurants();
  int tStart = millis(), delta;
  if (setting == quick) {
    qSort(rest_dist, len);
    delta = millis() - tStart;
    Serial.print("quicksort ");
  } else if (setting == insert) {
    iSort(rest_dist, len);
    delta = millis() - tStart;
    Serial.print("insertion sort ");
  }
  Serial.print("running time: ");
  Serial.print(delta); Serial.println(" ms");
  return len;
}