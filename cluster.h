#ifndef CLUSTER_H
#define CLUSTER_H

#include "position.h"
#include "union.h"
typedef struct {
  int size;
  int *members;
} Cluster;

Cluster *distribute_by_cluster(quick_union *map, int ncity);

void free_clusters(Cluster *clusters, int cluster_count);

int closest_city_outside_cluster(quick_union *map, position *coordinates,
                                 int ncity, int city);

int closest_city_to_cluster(quick_union *map, position *coordinates,
                            int ncity, int city);

#endif
