#ifndef CLUSTER_H
#define CLUSTER_H

#include "union.h"
typedef struct {
  int size;
  int *members;
} Cluster;

Cluster *distribute_by_cluster(quick_union *map, int ncity);

#endif
