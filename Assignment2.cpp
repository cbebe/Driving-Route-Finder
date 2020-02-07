//Made by Aaron Espiritu and Nick Hoskins

#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include <SD.h>

#include "lcd_image.h"

// standard U of A library settings, assuming Atmel Mega SPI pins
#define SD_CS    5  // Chip select line for SD card
#define TFT_CS   6  // Chip select line for TFT display
#define TFT_DC   7  // Data/command line for TFT
#define TFT_RST  8  // Reset line for TFT (or connect to +5V)

Sd2Card card;


#define JOY_VERT_ANALOG 0
#define JOY_HORIZ_ANALOG 1
#define JOY_SEL 9

#define MILLIS_PER_FRAME 25

// Only care about joystick movement if position is JOY_CENTRE +/- JOY_DEADZONE
#define JOY_DEADZONE 150
#define STEPS_PER_PIXEL 100 //analog reading must increase or decrease by 150 in value before
                          //cursor on screen reads one pixel
#define LCD_WIDTH 128
#define LCD_HEIGHT 160


const int NUM_RESTAURANTS = 1066;
const uint32_t REST_START_BLOCK = 4000000; //restaurant data starts at block 4 million

//restaurant struct
struct restaurant {
  int32_t lat;
  int32_t lon;
  uint8_t rating;
  char name[55];
};

struct RestDist{
  uint16_t index;     // index of restaurant from 0 to NUM_RESTAURANTS-1
  uint16_t dist;      // Manhatten distance to cursor position
};

RestDist rest_dist[NUM_RESTAURANTS];

uint32_t g_prev_block = 0;
restaurant g_buffer[8];
restaurant chosen_restaurant;


int g_joyX_center;
int g_joyY_center;
int cursorXposition = LCD_WIDTH / 2;
int cursorYposition = LCD_HEIGHT / 2;
int mapXposition;
int mapYposition;
int actualcursorX;
int actualcursorY;

int update = 0;
int button_update = 0;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

lcd_image_t map_image = { "yeg-big.lcd", 2048, 2048 };

void setup() {
  init();
  Serial.begin(9600);
  // enable pullup resistor for JOY_SELEC
  pinMode(JOY_SEL, INPUT);
  digitalWrite(JOY_SEL, HIGH);

  tft.initR(INITR_BLACKTAB);
  Serial.println("Initializing SD card...");

  //if image from SD card loads successfuly, print a statement of success
  if (!SD.begin(SD_CS)) {
    Serial.println ("Initialization failed...");
  }

  else Serial.println("Initialization successful!");

  //Initialize SD card and print if successsful
  if (!card.init(SPI_HALF_SPEED, SD_CS)) {
    Serial.println("failed!");
    while (true) {}
  }
  else {
    Serial.println("OK!"); Serial.println("");
  }

  //calibrate the joystick on startup
  g_joyY_center = analogRead(JOY_VERT_ANALOG);
  g_joyX_center = analogRead(JOY_HORIZ_ANALOG);


  //load up the map onto the LCD screen
  //when the image is loaded up, the top left corner of the image
  //and the top left corner of the LCD screen will matchup
  mapXposition = 1024;
  mapYposition = 1024;
  lcd_image_draw(&map_image, &tft , mapXposition, mapYposition, 0, 0, 128, 160);

  tft.drawLine(cursorXposition - 1 , cursorYposition, cursorXposition + 1, cursorYposition, ST7735_BLACK);
  tft.drawLine(cursorXposition - 1 , cursorYposition - 1, cursorXposition + 1, cursorYposition -1, ST7735_BLACK);
  tft.drawLine(cursorXposition - 1 , cursorYposition + 1, cursorXposition + 1, cursorYposition + 1, ST7735_BLACK);

  actualcursorX = cursorXposition + mapXposition;
  actualcursorY = cursorYposition + mapYposition;
}

//reprints map over previous cursor
void Cursor_erase() {

  //I had to keep the 'redrawing' of the map withing the confines of the crosshair or else
  //the map would make formatting errors near the edges of the LCD
  lcd_image_draw(&map_image, &tft, actualcursorX - 1 , actualcursorY - 1, cursorXposition - 1, cursorYposition - 1 ,3,3);

}

//function that scans for joystick movement and updates the screen accordingly
void Joy_scan () {
  int vertical = analogRead(JOY_VERT_ANALOG);
  int horizontal = analogRead(JOY_HORIZ_ANALOG);
  int select = digitalRead(JOY_SEL);
  //if analog stick is pushed past end zone, it adjusts position of cursor accordingly
  if (abs(vertical - g_joyY_center) > JOY_DEADZONE) {
    Cursor_erase();

    update = 1;


    int increment = (vertical - g_joyY_center) / STEPS_PER_PIXEL;
    //dont allow user to pass edge of map
      if (mapYposition == 0) {
      cursorYposition = constrain(cursorYposition + increment,0,170);
      }
      else if (mapYposition == 1888) {
      cursorYposition = constrain(cursorYposition + increment,-2,160);
      }
      else {cursorYposition = cursorYposition + increment;}

    //check to see if cursorY hits boundaries. If so then change map
    if (cursorYposition < 0) {
      mapYposition -= 64;
      if (mapYposition <= 0) {
        mapYposition = 0;
      }
      lcd_image_draw(&map_image, &tft , mapXposition, mapYposition, 0, 0, 128, 160);

      //set cursor back to middle of the screen
      cursorXposition = LCD_WIDTH / 2;
      cursorYposition = LCD_HEIGHT / 2;
      actualcursorX = cursorXposition + mapXposition;
      actualcursorY = cursorYposition + mapYposition;


    }

    if (cursorYposition > 160) {
      mapYposition += 64;
      if (mapYposition >= 1888) {
        mapYposition = 1888;
      }
      lcd_image_draw(&map_image, &tft , mapXposition, mapYposition, 0, 0, 128, 160);

      //set cursor back to middle of the screen
      cursorXposition = LCD_WIDTH / 2;
      cursorYposition = LCD_HEIGHT / 2;
      actualcursorX = cursorXposition + mapXposition;
      actualcursorY = cursorYposition + mapYposition;

    }
    actualcursorY = cursorYposition + mapYposition;
  }


  //if analog stick is pushed past end zone, it adjusts position of cursor accordingly
  if (abs(horizontal - g_joyX_center) > JOY_DEADZONE) {
    if (update != 1) {Cursor_erase();} //checks to see if Cursor_erased was called above already
    int increment = (horizontal - g_joyX_center) / STEPS_PER_PIXEL;
    //dont allow user to pass edge of map
      if (mapXposition == 0) {
      cursorXposition = constrain(cursorXposition + increment,0,170);
      }
      else if (mapXposition == 1920) {
      cursorXposition = constrain(cursorXposition + increment,-20,128);
      }
      else {cursorXposition = cursorXposition + increment;}


    //if cursor moves out of bounds, shift the map to the left
    if (cursorXposition < 0) {

      mapXposition -= 64;
      if (mapXposition <= 0) {
        mapXposition = 0;
      }
      lcd_image_draw(&map_image, &tft , mapXposition, mapYposition, 0, 0, 128, 160);

      //set cursor back to middle of the screen
      cursorXposition = LCD_WIDTH / 2;
      cursorYposition = LCD_HEIGHT / 2;
      actualcursorX = cursorXposition + mapXposition; //update actual positions
      actualcursorY = cursorYposition + mapYposition;
    }
    //if cursor moves out of bounds to the right, shift the map
    if (cursorXposition > 128) {
      mapXposition += 64;
      if (mapXposition >= 1920) {
        mapXposition = 1920;
      }
      lcd_image_draw(&map_image, &tft , mapXposition, mapYposition, 0, 0, 128, 160);

      //set cursor back to middle of the screen
      cursorXposition = LCD_WIDTH / 2;
      cursorYposition = LCD_HEIGHT / 2;
      actualcursorX = cursorXposition + mapXposition; //update actual positions
      actualcursorY = cursorYposition + mapYposition;
    }

    actualcursorX = cursorXposition + mapXposition;
    update = 1;
  }

  //check to see if analog stick is pressed down
  if (select == LOW) {
    button_update = 1;

  }
}

//update cursor onto screen as analog stick is pushed
void Cursor_update () {

  tft.drawLine(cursorXposition - 1 , cursorYposition, cursorXposition + 1, cursorYposition, ST7735_BLACK);
  tft.drawLine(cursorXposition - 1 , cursorYposition - 1, cursorXposition + 1, cursorYposition -1, ST7735_BLACK);
  tft.drawLine(cursorXposition - 1 , cursorYposition + 1, cursorXposition + 1, cursorYposition + 1, ST7735_BLACK);


}


void get_restaurant_fast(restaurant* ptr, int i) {

  //starts at block 4 million and locates block with restaurant
  uint32_t block = REST_START_BLOCK + (i / 8);
  if (block == g_prev_block) {

    *ptr = g_buffer[i % 8]; // global variable array was saved and is now re-used

  }

  //else use the same function as above
  else {
    // block - the block we want to read (uint32_t)
    // g_buffer - address in ram we want to put the block
    //          a pointer: uint8_t*
    if (!card.readBlock(block, (uint8_t*) g_buffer)) { //checks to see if reading is successful
    Serial.println("Read block failed!");
    while (true) {}
  }

  // now g_buffer holds the restaurants from block "block"

  *ptr = g_buffer[i%8]; // the correct restaurant
  }

  //update previous block value
  g_prev_block = block;


  }


  // These constants are for the 2048 by 2048 map.

const int16_t map_width = 2048;
const int16_t map_height = 2048;
const int32_t lat_north = 5361858;
const int32_t lat_south = 5340953;
const int32_t lon_west = -11368652;
const int32_t lon_east = -11333496;

  // These functions convert between x/y map position and lat/lon
  // (and vice versa.)
  int32_t x_to_lon(int16_t x) {
      return map(x, 0, map_width, lon_west, lon_east);
  }

  int32_t y_to_lat(int16_t y) {
      return map(y, 0, map_height, lat_north, lat_south);
  }

  int16_t lon_to_x(int32_t lon) {
      return map(lon, lon_west, lon_east, 0, map_width);
  }

  int16_t lat_to_y(int32_t lat) {
      return map(lat, lat_north, lat_south, 0, map_height);
  }

//calculate distance given the necessary paramaters for x and y
uint32_t  getdistance (int actualcursorX,int32_t lon,int actualcursorY,int32_t lat) {
  lon = lon_to_x(lon); //convert lon and lat values to x-y coordinates
  lat = lat_to_y(lat);
  uint32_t distance = abs(actualcursorX - lon) + abs(actualcursorY - lat);
  return distance;
}





// Swap two restaurants of RestDist struct
void swap_rest(RestDist *ptr_rest1, RestDist *ptr_rest2) {
  RestDist tmp = *ptr_rest1;
  *ptr_rest1 = *ptr_rest2;
  *ptr_rest2 = tmp;
}

// Selection sort
// rest_dist is an array of RestDist, from rest_dist[0] to rest_dist[len-1]
void ssort(RestDist *rest_dist, int len) {
  for (int i = len-1; i >= 1; --i) {
    // Find the index of furthest restaurant
    int max_idx = 0;
    for (int j = 1; j <= i; ++j) {
      if(rest_dist[j].dist > rest_dist[max_idx].dist) {
        max_idx = j;
      }
    }
    // Swap it with the last element of the sub-array
    swap_rest(&rest_dist[i], &rest_dist[max_idx]);
  }
}



void fill_distarray(RestDist *dist) {

  for (int i = 0; i < NUM_RESTAURANTS; i++) {
    dist[i].index = i;
    get_restaurant_fast(&chosen_restaurant, i);
    dist[i].dist = getdistance(actualcursorX, chosen_restaurant.lon, actualcursorY, chosen_restaurant.lat);
  }

}

void Mode_1 () {

  fill_distarray(rest_dist);
  int time_1 = millis();
  ssort(rest_dist, NUM_RESTAURANTS);
  int time_2 = millis();
  Serial.print("Time to sort: "); Serial.println(time_2 - time_1);


 tft.fillScreen(0);
 tft.setCursor(0, 0); // where the characters will be displayed
 tft.setTextWrap(false);
 int selected_rest = 0; // which restaurant is selected
 for (int i = 0; i < 20; i++) {
   restaurant r;
   get_restaurant_fast(&r, rest_dist[i].index);
   if (i != selected_rest) { // not highlighted
     tft.setTextColor(0xFFFF, 0x0000); // white characters on black background

   } else { // highlighted
     tft.setTextColor(0x0000, 0xFFFF); // black characters on white background
     actualcursorX = lon_to_x(r.lon);
     actualcursorY = lat_to_y(r.lat);
   }
   tft.print(r.name);
   tft.print("\n");
 }

 int select;
 int exitkey = 0;
 while(exitkey == 0) {

   int vertical = analogRead(JOY_VERT_ANALOG);

   if (abs(vertical - g_joyY_center) > JOY_DEADZONE) {
     restaurant r;
     //reprint current restaurant black
     get_restaurant_fast(&r, rest_dist[selected_rest].index);
     tft.setCursor(0,8 * selected_rest);
     tft.setTextColor(0xFFFF, 0x0000);
     tft.print(r.name);

     if (vertical - g_joyY_center < 0) {
       //the user wont be able to scroll past boundaries of screen
       selected_rest = constrain(selected_rest - 1, 0 , 19);
     }

     else if (vertical - g_joyY_center > 0) {
       selected_rest = constrain(selected_rest+ 1, 0 , 19);
     }
     //get the name of the new restaurant that will be highlighted white
     get_restaurant_fast(&r, rest_dist[selected_rest].index);
     tft.setCursor(0,8 * selected_rest);
     //highlight new selected restaurant
      tft.setTextColor(0x0000, 0xFFFF);
       tft.print(r.name);

       //set the cursor positions to xy coordinates of selected restaurant
       actualcursorX = lon_to_x(r.lon);
       actualcursorY = lat_to_y(r.lat);

       delay(150);

     }





   //if button is pressed again, exit the menu and return to screen with
   //selected restaurant at center of map.
   select = digitalRead(JOY_SEL);
   if(select == LOW) {
     exitkey = 1;
     button_update = 0;

     /*map coordinates will be shifted accordingly so that the restaurant coordinates
     are in the middle of the screen. As well, the cursor
     will also be redrawn in the middle of the screen  */
     cursorXposition = LCD_WIDTH / 2;
     cursorYposition = LCD_HEIGHT / 2;

     mapXposition = actualcursorX - (LCD_WIDTH /2);
     mapYposition = actualcursorY - (LCD_HEIGHT / 2);

     if (mapYposition <= 0) {
       mapYposition = 0;
       if (actualcursorY <= 0) {
         actualcursorY = 0;
         cursorYposition = 0;
       } else {
         cursorYposition = actualcursorY;
       }
     }
     if (mapYposition >= 1888) {
       mapYposition = 1888;
       if (actualcursorY >= 2048) {
         actualcursorY = 2048;
         cursorYposition = 160;
       }
       else {
         cursorYposition = actualcursorY;
       }
     }
     if (mapXposition <= 0) {
       mapXposition = 0;
       if (actualcursorX <= 0) {
         actualcursorX = 0;
         cursorXposition = 0;
       }
       else {
         cursorXposition = actualcursorX;
       }
     }
     if (mapXposition >= 1920) {
       mapXposition = 1920;
       if (actualcursorX >= 2048) {
         actualcursorX = 2048;
         cursorXposition = 128;
       }
       else {
         cursorXposition = actualcursorX;
       }
     }


     lcd_image_draw(&map_image, &tft , mapXposition, mapYposition, 0, 0, 128, 160);
     tft.drawLine(cursorXposition - 1 , cursorYposition, cursorXposition + 1, cursorYposition, ST7735_BLACK);
     tft.drawLine(cursorXposition - 1 , cursorYposition - 1, cursorXposition + 1, cursorYposition -1, ST7735_BLACK);
     tft.drawLine(cursorXposition - 1 , cursorYposition + 1, cursorXposition + 1, cursorYposition + 1, ST7735_BLACK);



   }
 }
 tft.print("\n");

 }


//these functions are just used to test other subfunctions
void fill_test() {
  fill_distarray(rest_dist);
  for (int i = 0; i < NUM_RESTAURANTS; i++) {
    Serial.print("Index: "); Serial.println(rest_dist[i].index);
    Serial.print("Distance: "); Serial.println(rest_dist[i].dist);
  }
} //test that prints indices and distances to serial monitor
void ssort_test() {
  ssort(rest_dist, NUM_RESTAURANTS);
  for (int i= 0; i < NUM_RESTAURANTS; i++) {
    Serial.print("Restaurant: "); Serial.println(rest_dist[i].index);
    Serial.print("Distance: "); Serial.println(rest_dist[i].dist);
    Serial.print("counter : "); Serial.print(i);
  }
} //test that ssort works and print to serial monitor //test to see if sorting works
void get_restaurant_test() {
  get_restaurant_fast(&chosen_restaurant, 10);
  Serial.println(chosen_restaurant.lat);
  Serial.println(chosen_restaurant.lon);
   Serial.println(chosen_restaurant.name);

   get_restaurant_fast(&chosen_restaurant, 45);
   Serial.println(chosen_restaurant.lat);
   Serial.println(chosen_restaurant.lon);
    Serial.println(chosen_restaurant.name);

    get_restaurant_fast(&chosen_restaurant, 320);
    Serial.println(chosen_restaurant.lat);
    Serial.println(chosen_restaurant.lon);
     Serial.println(chosen_restaurant.name);
     get_restaurant_fast(&chosen_restaurant, 850);
     Serial.println(chosen_restaurant.lat);
     Serial.println(chosen_restaurant.lon);
      Serial.println(chosen_restaurant.name);
      get_restaurant_fast(&chosen_restaurant, 1065);
      Serial.println(chosen_restaurant.lat);
      Serial.println(chosen_restaurant.lon);
       Serial.println(chosen_restaurant.name);
       get_restaurant_fast(&chosen_restaurant, 0);
       Serial.println(chosen_restaurant.lat);
       Serial.println(chosen_restaurant.lon);
      Serial.println(chosen_restaurant.name);




} //another test

int main() {
  setup();


  while(true) {
    Joy_scan();
    //print new and erase previous cursor
    if (update == 1) {
      //redraw over old cursor and update LCD to display new cursor
      Cursor_update();

    }
    if(button_update == 1) {
      button_update = 0;
      Mode_1();
    }

    int t = millis();
    int prevTime;

    //this will delay the time between updates to slow down cursor movement and make
    //it smoother. The delay is modified to allow smooth movement of cursor
    if (t - prevTime < MILLIS_PER_FRAME) {
      delay(MILLIS_PER_FRAME - (t - prevTime));
    }
    prevTime = millis();
  update = 0;
  }

  return 0;
}
