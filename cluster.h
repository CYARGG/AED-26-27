#ifndef CLUSTER_H
#define CLUSTER_H

#include "position.h"
#include "union.h"
typedef struct {
  int size;
  int *members;
  int closest_outside; /* resposta da Task4 em cache, 0 = ainda não calculada */
} Cluster;

typedef struct {
  Cluster *clusters;    /* índices 1 a count, a posição 0 não é usada */
  int *cluster_of_root; /* raiz do union-find -> índice em clusters */
  int count;
} ClusterSet;

ClusterSet *distribute_by_cluster(quick_union *map, int ncity);

void free_clusters(ClusterSet *set);

int closest_city_outside_cluster(quick_union *map, const ClusterSet *set,
                                 position *coordinates, int ncity, int city);

int closest_city_to_cluster(quick_union *map, ClusterSet *set,
                            position *coordinates, int ncity, int city);

#endif
