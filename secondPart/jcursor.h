#define JOY_VERT  A9 // should connect A9 to pin VRx
#define JOY_HORIZ A8 // should connect A8 to pin VRy

#define JOY_CENTRE 512
#define JOY_DEADZONE 64
#define POS_BUFFER (JOY_CENTRE + JOY_DEADZONE)
#define NEG_BUFFER (JOY_CENTRE - JOY_DEADZONE)
#define SPEED_DIV 64 // 32 for normal people, 64 for masochists
#define CURSOR_SIZE  9
#define CUR_RAD 4


extern int cursorX, cursorY, currentPatchX, currentPatchY;

void mapInit();
void processJoystick();
void setup();
