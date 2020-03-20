/*
    Graphics library test for MCU Friend 3.5" TFT LCD shield.

    Tests out various functions from the graphics library.
*/

#include <Arduino.h>

// core graphics library for displays (written by Adafruit)
#include <Adafruit_GFX.h>

// necessary for using a GFX font
#include <Fonts/FreeSansBold9pt7b.h>

// LCD and SD card will communicate using the Serial Peripheral Interface (SPI)
// e.g., SPI is used to display images stored on the SD card
// #include <SPI.h>

// needed for reading/writing to SD card
// #include <SD.h>

// define constants using #define preprocessor
// #define LCD_CS A3 // Chip Select goes to Analog 3
// #define LCD_CD A2 // Command/Data goes to Analog 2
// #define LCD_WR A1 // LCD Write goes to Analog 1
// #define LCD_RD A0 // LCD Read goes to Analog 0
// #define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
// #define SD_CS 10  // SD chip select pin

// Graphics library for MCU Friend 3.5" TFT LCD shield
#include <MCUFRIEND_kbv.h>

// make an object from MCUFRIEND_kbv class
MCUFRIEND_kbv tft;
//MCUFRIEND_kbv tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// Assign human-readable names to some common 16-bit color values (UTFT color code)
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void setup() {
	init();
    Serial.begin(9600);

    //    tft.reset();             // hardware reset
    uint16_t ID = tft.readID();    // read ID from display
    Serial.print("ID = 0x");
    Serial.println(ID, HEX);
    if (ID == 0xD3D3) ID = 0x9481; // write-only shield
    //    ID = 0x9329;             // force ID

    tft.begin(ID);                 // LCD gets ready to work
}

void tftDemo() {
    // rotate 0, 90, 180 or 270 degrees
    // the rotation parameter can be 0 (tall or PORTRAIT), 
    // 1 (wide or LANDSCAPE), 2 (tall or PORTRAIT_REV) or 3 (wide or LANDSCAPE_REV)
    // default is 0 which sets the display to a portrait (tall) mode
    // 
    tft.setRotation(1);  

    // set the color of the whole screen to BLACK.
    tft.fillScreen(BLACK);

    // print the display size using println
    // println moves the cursor to the next line after printing the string
    tft.println(tft.width());  // width of the screen
    tft.println(tft.height()); // height of the screen

    // draw a single character, default font size is 5x8 pixels
    // coordinates are always expressed in pixel units
    tft.drawChar(200,100,'T',WHITE,BLACK,3);

    // draw a single dot (or pixel) on the display with a specified color
    tft.drawPixel(50,50,WHITE);

    // read the color of a pixel in x and y location
    uint16_t cpixel = tft.readPixel(50,50);
    
    // draw a line of the specified color 
    // with a starting and ending point that you provide
    tft.drawLine(50,50,100,100,WHITE);
    
    // draw a horizontal line (that goes from left to right starting from x and y) 
    // of the length and color you specify
    tft.drawFastHLine(50,50,50,GREEN);

    // draw a vertical line (that goes downward starting from x and y) 
    // of the length and color you specify
    tft.drawFastVLine(50,50,150,GREEN);

    // the TFT display can display up to 65,536 colors (16 bits)
    // hence, you have to convert 24-bit RGB color to RGB565 16-bit color
    // draw a rectangle to the TFT screen, 
    // the first two parameters determine the top left corner of the rectangle
    tft.drawRect(100,100,20,20,tft.color565(255,127,127));
    
    // draw a solid rectangle to the TFT screen
    tft.fillRect(200,200,10,10,tft.color565(255,0,0));
    
    // draw a circle to the TFT screen
    tft.drawCircle(100,100,20,YELLOW);

    // draw a filled circle to the TFT screen
    tft.fillCircle(300,300,20,GREEN);

    // here is another way to display text

    // place the top left corner of the text 
    // at the specified location
    tft.setCursor(100,100);

    tft.println(tft.width());  // width of the screen
    tft.println(tft.height()); // height of the screen

    tft.setCursor(100,200);
    // set the text color (background color is optional)
    tft.setTextColor(WHITE, BLACK);
    // multiply the scale of the text by a given integer factor (between 1 and 5)
    tft.setTextSize(1);
    // change the font
    tft.setFont(&FreeSansBold9pt7b);

    // print text without moving the cursor to the next line
    tft.print("CMPUT 275");

	Serial.println("Finished the demo!");
}

int main() {
	setup();

	tftDemo();

	Serial.end();

	return 0;
}