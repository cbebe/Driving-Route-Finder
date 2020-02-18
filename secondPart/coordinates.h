#define JOY_SEL 53 

#define TFT_WIDTH 480
#define MAP_WIDTH 420
#define BND_WIDTH (MAP_WIDTH -5)

#define TFT_HEIGHT 320
#define MAP_HEIGHT 320
#define BND_HEIGHT (MAP_HEIGHT -5)

#define YEG_SIZE 2048
#define MAP_MAXX (YEG_SIZE - MAP_WIDTH)
#define MAP_MAXY (YEG_SIZE - MAP_HEIGHT)

// width/height of the display when rotated horizontally
#define TFT_WIDTH 480
#define TFT_HEIGHT 320

// layout of display for this assignment,
#define RATING_SIZE 60
#define DISP_WIDTH (TFT_WIDTH - RATING_SIZE)
#define DISP_HEIGHT TFT_HEIGHT

#define CENTRE_X (MAP_WIDTH/2)
#define CENTRE_Y (MAP_HEIGHT/2)
#define MAX_X (YEG_SIZE - CENTRE_X)
#define MAX_Y (YEG_SIZE - CENTRE_Y)

#define LAT_NORTH 5361858
#define LAT_SOUTH 5340953
#define LON_WEST -11368652
#define LON_EAST -11333496

#define REST_START_BLOCK 4000000
#define NUM_RESTAURANTS 1066

#define SD_CS 10

struct restaurant {
  int32_t lat;
  int32_t lon;
  uint8_t rating; // from 0 to 10
  char name[55];
};

enum sort {quick, insert, both};

struct RestDist {
  uint16_t index;
  uint16_t dist;
};


extern sort sortSetting;
extern RestDist rest_dist[NUM_RESTAURANTS];

// global declaration to give access to multiple source files
extern MCUFRIEND_kbv tft; 
extern uint32_t pastBlockNum;
extern restaurant restBlock[8];
extern Sd2Card card;
extern int cursorX, cursorY, currentPatchX, currentPatchY;
extern int8_t ratingSel;

void getRestaurantFast(int restIndex, restaurant *restPtr);

int runSort(sort setting);