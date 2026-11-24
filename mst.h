#ifndef MST_H
#define MST_H
#include "cluster.h"
#include "position.h"

typedef struct mst mst;

typedef struct connection connection;

mst *mst_initialize(const ClusterSet *set, const position *coordinates,
                    int ncity);

void mst_cleanup(mst *tree);

int mst_get_count(const mst *tree);

double mst_get_total(const mst *tree);

int mst_get_start(const mst *tree, int i);

int mst_get_ending(const mst *tree, int i);

double mst_get_distance(const mst *tree, int i);
#endif
