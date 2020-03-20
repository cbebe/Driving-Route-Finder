/*
  A demonstration of how we can fetch restaurant data from the SD card
  by reading one block at a time.

  Also has some examples of how to use structs.
*/

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SD.h>

#define SD_CS 10

#define REST_START_BLOCK 4000000    // address of the first restaurant data
#define NUM_RESTAURANTS 1066        // total number of restaurants

MCUFRIEND_kbv tft;

// Sd2Card class has methods for raw access 
// to SD and SDHC flash memory cards
Sd2Card card;

struct restaurant { // 64 bytes
  int32_t lat;
  int32_t lon;
  uint8_t rating;   // from 0 to 10
  char name[55];
};

void setup() {
  init();
  Serial.begin(9600);

  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.fillScreen(TFT_BLACK);

  // The following initialization (commented out) is
  // not needed for just raw reads from the SD card, but you should add it later
  // when you bring the map picture back into your assignment
  // (both initializations are required for the assignment)

  // initialize the SD card
  /*
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("failed! Is the card inserted properly?");
    while (true) {}
  }
  else {
    Serial.println("OK!");
  }
  */

  // initialize SPI (serial peripheral interface)
  // communication between the Arduino and the SD controller

  Serial.print("Initializing SPI communication for raw reads...");
  if (!card.init(SPI_HALF_SPEED, SD_CS)) {
    Serial.println("failed! Is the card inserted properly?");
    while (true) {}
  }
  else {
    Serial.println("OK!");
  }
}

void testStructAlignment() {
  restaurant rest;

  if ((int) &rest != (int) &rest.lat) {
    Serial.println("latitude not at start");
  }
  else if (((int) &rest) + 4 != (int) &rest.lon) {
    Serial.println("longitude not 4 bytes from the start");
  }
  else if (((int) &rest) + 8 != (int) &rest.rating) {
    Serial.println("rating not 8 bytes from the start");
  }
  else if (((int) &rest) + 9 != (int) &rest.name) {
    Serial.println("name not 9 bytes from the start");
  }
  else {
    Serial.println("restaurant struct alignment is ok");
  }
}

// read data of one restaurant (indexed by "restIndex") 
// from the raw partition in the card and load it into
// a struct pointed to by restPtr
void getRestaurant(int restIndex, restaurant* restPtr) {
  // calculate the block containing this restaurant
  uint32_t blockNum = REST_START_BLOCK + restIndex/8;
  restaurant restBlock[8];

  // Serial.println(blockNum);

  // fetch a block (512B) of restaurants data including 
  // data about the restaurant indexed by "restIndex"
  while (!card.readBlock(blockNum, (uint8_t*) restBlock)) {
    Serial.println("Read block failed, trying again.");
  }
  // Serial.print("Loaded: ");
  // Serial.println(restBlock[0].name);

  *restPtr = restBlock[restIndex % 8];
}

void testRestaurantStruct() {
  // can initialize a struct by specifying the values of its components
  // in the same order they are listed in the struct declaration
  // if a struct is partially initialized, 
  // all its uninitialized members are implicitly initialized to zero
  restaurant rest = { 123, 456, 9, "Tim Horton's" };

  // accessing struct members
  Serial.println(rest.lat);
  Serial.println(rest.lon);
  Serial.println(rest.rating);
  Serial.println(rest.name);

  rest.lat = -747;
  Serial.println(rest.lat);

  Serial.print("size of the restaurant struct: ");
  Serial.println(sizeof(rest));

  // initilizing by copying (using memcpy()) member values from rest to rest1
  restaurant rest2 = rest;
  Serial.println(rest2.name);

  // copy the characters of the string "Subway" into the 55 characters
  // of the rest2 struct
  strcpy(rest2.name, "Subway");
  Serial.println(rest2.name);

}

int main() {
  setup();

  testStructAlignment();

  // testRestaurantStruct();

  // now start reading restaurant data, let's do the first block now
  restaurant restBlock[8]; // 512 bytes in total: a block

  Serial.println();
  Serial.println("Now reading restaurants");

  // restBlock is a uint8_t pointer to the mem location where
  // a 512-byte block will be stored
  // why uint8_t? because this function expects 
  // a uint8_t pointer to the start of the block
  card.readBlock(REST_START_BLOCK, (uint8_t*) restBlock);

  // print all restaurant names in this block
  for (int i = 0; i < 8; ++i) {
    Serial.println(restBlock[i].name);
  }

  Serial.println();
  Serial.println("Fetching the first 20 restaurants");
  restaurant rest3;
  for (int i = 0; i < 20; ++i) {
    getRestaurant(i, &rest3);
    Serial.print(i);
    Serial.print(" ");
    Serial.println(rest3.name);
  }

  Serial.end();

  return 0;
}
