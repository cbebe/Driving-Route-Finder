#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>
#include "lcd_image.h"
#include "mode0.h"
#include "coordinates.h"

MCUFRIEND_kbv tft;
int cursorX, cursorY, currentPatchX, currentPatchY;