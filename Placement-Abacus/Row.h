#ifndef _ROW_H_
#define _ROW_H_
#include <bits/stdc++.h>
#include "Cluster.h"
struct Row
{
    int CoordY;
    int CoordX;
    int Height;
    int NumSites;
    int Width;
    int Xmin, Xmax;
    int SiteSpacing;
    int totalCellWidth;
    std::vector<int> nodeVec;
    Cluster* cluster;
    Row():CoordY(0),CoordX(0),Height(0),Width(0),SiteSpacing(0),NumSites(0), cluster(NULL), totalCellWidth(0) {};
    void printRow();
    // Row operator=(const Row& R) {
    //     CoordX = R.CoordX;
    //     CoordY = R.CoordY;
    //     Height = R.Height;
    //     SiteSpacing = R.SiteSpacing;
    //     return (*this);
    // } 
};

void Row::printRow() {
    std::cout<<"Bottom Coordinate Y: "<<CoordY<<" Left Coordinate X: "<<CoordX<<" Height: "<<Height<<" Width: "<<Width<<" SiteSpacing(Width): "<<SiteSpacing<<" NumSites: "<<NumSites<<" Xmax: "<<Xmax<<"\n";
}

#endif