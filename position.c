#include "position.h"
#include <stdbool.h>

bool out_of_bound(int id, int x, int y, int ncity, int limx, int limy) {
  if (x > limx || x < 1 || y > limy || y < 1 || id < 1 || id > ncity) {
    return true;
  }
  return false;
}

/* distância euclidiana ao quadrado (chega para comparar distâncias) */
long squared_distance(position a, position b) {
  long dx = (long)a.x - b.x;
  long dy = (long)a.y - b.y;
  return dx * dx + dy * dy;
}
