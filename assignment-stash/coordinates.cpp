#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>
#include "lcd_image.h"
#include "coordinates.h"

MCUFRIEND_kbv tft;
int cursorX, cursorY, currentPatchX, currentPatchY;
Sd2Card card;
uint32_t pastBlockNum;
restaurant restBlock[8];
RestDist rest_dist[NUM_RESTAURANTS];

void getRestaurantFast(int restIndex, restaurant *restPtr) {
  uint32_t blockNum = REST_START_BLOCK + restIndex/8;
  // only loads from SD card when not on the same block
  if (blockNum != pastBlockNum) {
    while (!card.readBlock(blockNum, (uint8_t*) restBlock)) {
      Serial.println("Read block failed, trying again.");
    }
    pastBlockNum = blockNum;
  }
  *restPtr = restBlock[restIndex % 8];
}