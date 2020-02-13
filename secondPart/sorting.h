enum sort {quick, insert, both};

struct RestDist {
  uint16_t index;
  uint16_t dist;
};

extern sort sortSetting;
extern RestDist rest_dist[NUM_RESTAURANTS];

void runSort(sort setting);