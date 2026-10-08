#include "union.h"
#include <stdlib.h>

struct quick_union {
  int *parent;
  int *size;
  int ncity;
  int cluster_counter;
};

quick_union *initialize_quick_union(int ncity) {

  quick_union *map = malloc(sizeof(quick_union));
  if (map == NULL) {
    return NULL;
  }
  map->parent = malloc((ncity + 1) * sizeof(int));
  if (map->parent == NULL) {
    free(map);
    return NULL;
  }
  map->size = malloc((ncity + 1) * sizeof(int));
  if (map->size == NULL) {
    free(map->parent);
    free(map);
    return NULL;
  }
  map->ncity = ncity;
  map->cluster_counter = ncity;

  for (int i = 1; i <= ncity; i++) {
    map->parent[i] = i;
    map->size[i] = 1;
  }
  return map;
}

int find(quick_union *map, int city) {
  int current_city = city;
  int tmp, root;

  while (map->parent[city] != city) {
    city = map->parent[city];
  }
  root = city;
  while (current_city != root) {
    tmp = map->parent[current_city];
    map->parent[current_city] = root;
    current_city = tmp;
  }
  return root;
}

void unite(quick_union *map, int city_1, int city_2) {
  int root_1 = find(map, city_1);
  int root_2 = find(map, city_2);
  if (root_1 != root_2) {
    map->cluster_counter -= 1;
    if (map->size[root_1] > map->size[root_2]) {
      map->parent[root_2] = root_1;
      map->size[root_1] += map->size[root_2];

    } else {
      map->parent[root_1] = root_2;
      map->size[root_2] += map->size[root_1];
    }
  }
}

int get_cluster_count(const quick_union *map) { return map->cluster_counter; }

int get_cluster_size(quick_union *map, int city) {
  return map->size[find(map, city)];
}
