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
#define NUM_LINES 21

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

// global declaration to give access to multiple source files
extern MCUFRIEND_kbv tft; 
extern Sd2Card card;

extern RestDist rest_dist[NUM_RESTAURANTS]; // RestDist array to be sorted
extern uint32_t pastBlockNum;
extern restaurant restBlock[8]; // cached SD card block
// coordinates of current cursor position
extern int cursorX, cursorY, currentPatchX, currentPatchY;

// button selection variables
extern sort sortSetting;
extern int8_t ratingSel;

extern int16_t selectedRest; // state variables for restlist
extern int8_t pageNum;

extern restaurant currentRest; // for caching restaurant data

// function declarations
void getRestaurantFast(int restIndex, restaurant *restPtr);
int runSort(sort setting);
void joySelect(int prevRest, int len);
void pageUpdate(bool top);
void restList();
