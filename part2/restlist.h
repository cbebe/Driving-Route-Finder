#define JOY_SEL 53
#define NUM_LINES 21

struct RestDist {
  uint16_t index;
  uint16_t dist;
};

void restList();
void joySelect(int prevRest);
void goToResto();
extern int8_t selectedRest;
extern RestDist rest_dist[NUM_RESTAURANTS];
