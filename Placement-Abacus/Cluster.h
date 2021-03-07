#ifndef _CLUSTER_H_
#define _CLUSTER_H_
#include <bits/stdc++.h>
struct Cluster {
    Cluster* next, *last;
    int n_first, n_last;
    int NumCell;
    int x_c, e_c, w_c, q_c;
    int Xmin, Xmax;
    std::vector<int> cellVec;
    Cluster():n_first(0), n_last(0), NumCell(0), x_c(0), e_c(0), w_c(0), q_c(0), Xmin(INT_MAX), Xmax(INT_MIN), last(this), next(NULL){}
};


#endif