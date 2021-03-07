#ifndef _VERTEX_H_
#define _VERTEX_H_
#include <bits/stdc++.h>
#include "Cluster.h"
struct vertex
{
    int Height;
    int Width;
    int Weight;
    bool fixed;
    bool overlap;
    std::string vertexName;
    Cluster* cluster;
    std::pair<int,int> Coord;
    std::pair<int,int> TmpCoord;
    std::pair<int,int> PlacedCoord;
    vertex():Height(0), Width(0), Weight(1), fixed(false), overlap(false), cluster(NULL){}
    void printVertex();
    bool operator < (const vertex& v) const {
        return Coord.first < v.Coord.first;
    }
};

void vertex::printVertex() {
    std::cout<<"Vertex Name:"<<vertexName<<" Left Bottom Coordinate: ("<<Coord.X<<","<<Coord.Y<<") Height:"<<Height<<" Width: "<<Width<<" ";
    if (fixed)
        std::cout<<"Fixed\n";
    else 
        std::cout<<"Flexible\n";
}
#endif