#ifndef TASKS_H
#define TASKS_H

typedef struct {
  int size;
  int *members;
} Cluster;

Cluster *task_2(int parent[], int cluster_number, int ncity);

#endif
