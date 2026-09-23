#include "union.h"
#include <stdio.h>

int find(int parent[], int city) {
  while (parent[city] != city) {
    city = parent[city];
  }
  return city;
}

void unite(int parent[], int size[], int city_1, int city_2) {
  int root_1 = find(parent, city_1);
  int root_2 = find(parent, city_2);
  if (root_1 != root_2) {

    if (size[root_1] > size[root_2]) {
      parent[root_2] = root_1;
      size[root_1] += size[root_2];

    } else {
      parent[root_1] = root_2;
      size[root_2] += size[root_1];
    }
  }
  return;
}
