#ifndef _VERTEX_H_
#define _VERTEX_H_
#include <vector>
struct vertex {
    bool moved;
    bool group;
    int gain;
    int FS;
    int TE;
    std::set<int> net;
    vertex():moved(false),group(false),gain(0), FS(0), TE(0){}
};
#endif