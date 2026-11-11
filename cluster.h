#ifndef CLUSTER_H
#define CLUSTER_H

#include "position.h"
#include "union.h"

typedef struct Cluster Cluster; /*TODO: pode sair daqui e ir para o .c*/

typedef struct ClusterSet ClusterSet;

ClusterSet *distribute_by_cluster(quick_union *map, int ncity);

void free_clusters(ClusterSet *set);

int closest_city_outside_cluster(quick_union *map, const ClusterSet *set,
                                 position *coordinates, int ncity, int city);

int closest_city_to_cluster(quick_union *map, ClusterSet *set,
                            position *coordinates, int ncity, int city);

int get_number_of_clusters(const ClusterSet *set);

int get_cluster_specific_size(
    const ClusterSet *set, int cluster); /*indices de 1 até count, salta o 0*/

int get_cluster_member_by_id(
    const ClusterSet *set,
    int cluster, /* cluster vai de 1 a count e indice de 0 a size-1 */
    int indice_member);
#endif
