enum sort {quick, insert, both};

extern sort sortSetting;

struct RestDist {
  uint16_t index;
  uint16_t dist;
};

extern RestDist rest_dist[NUM_RESTAURANTS];

void runSort(sort setting);