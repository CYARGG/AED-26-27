#ifndef POSITION_H
#define POSITION_H
#include <stdbool.h>

typedef struct {
  int x;
  int y;

} position;

bool out_of_bound(int id, int x, int y, int ncity, int limx, int limy);

#endif
