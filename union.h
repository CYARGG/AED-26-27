#ifndef UNION_H
#define UNION_H

typedef struct quick_union quick_union;

int find(quick_union *map, int city);

void unite(quick_union *map, int city_1, int city_2);

quick_union *initialize_quick_union(int ncity);

int get_cluster_count(const quick_union *map);

int get_cluster_size(quick_union *map, int city);

#endif
