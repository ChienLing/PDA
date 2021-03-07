#ifndef _PARSER_H_
#define _PARSER_H_
#define X first
#define Y second
#include <bits/stdc++.h>
#include "vertex.h"
#include "Row.h"
class Placement;
class Parser {
    public:
    Placement *pm;
    std::ifstream auxFile, plFile, nodeFile, sclFile;
    Parser();
    Parser(Placement* p):pm(p){ }
    void readInput(const std::string);
    void readPl();
    void readScl();
    void readNode();
};

Parser::Parser(){}


void Parser::readInput(const std::string auxName) {
    std::cout<<"read input\n";//\nauxName: "<<auxName<<"\n";
    auxFile.open(auxName);
    if (!auxFile) {
        std::cerr<<"Can not open "<<auxName<<"!\n";
        exit(1);
    }
    std::string nodeName, netsName, wtsName, plName, sclName, shapesName;
    std::string trash;
    auxFile>>trash>>trash>>nodeName>>netsName>>wtsName>>plName>>sclName>>shapesName;
    //std::cout<<nodeName<<" "<<netsName<<" "<<wtsName<<" "<<plName<<" "<<sclName<<" "<<shapesName<<"\n";
    plFile.open(plName);
    if (!plFile) {
        std::cerr<<"Can not open "<<plName<<"!\n";
        exit(1);
    }
    nodeFile.open(nodeName);
    if (!nodeFile) {
        std::cerr<<"Can not open "<<nodeName<<"!\n";
        exit(1);
    }
    sclFile.open(sclName);
    if (!sclFile) {
        std::cerr<<"Can not open "<<sclName<<"!\n";
        exit(1);
    }
    readPl();
    readScl();
    readNode();
}

void Parser::readPl() {
    std::cout<<"Read PL\n";
    std::string S, nodeName, trash;
    int CoordX, CoordY;
    for (int i=0; i<4; i++) 
        getline(plFile,S);
    long long idx(0);
    while (getline(plFile,S)) {
        std::stringstream SS(S);
        vertex v;
        SS>>nodeName>>CoordX>>CoordY>>trash>>trash;
        if (SS>>trash) 
            v.fixed = true;
        v.Coord.X = CoordX;
        v.Coord.Y = CoordY;
        v.vertexName = nodeName;
        pm->nodeNameToIdx[nodeName] = idx;
        pm->vertexVec.push_back(v);
        idx++;
    }
    // pm->printVertexVec();
}

void Parser::readScl() {
    std::string S, trash, name;
    for (int i=0; i<5; i++) 
        getline(sclFile,S);
    
    {
        std::stringstream SS(S);
        SS>>trash>>trash>>pm->NumRow;
    }
    pm->InitRowVec.resize(pm->NumRow);
    pm->RowCut.resize(pm->NumRow);
    pm->rowVec.resize(pm->NumRow);
    // while (getline(sclFile, S)) {
    for (int i=0; i<pm->NumRow; i++) {
        Row R;
        while (getline(sclFile, S)) {
            std::stringstream SS(S);
            SS>>name;
            // std::cout<<"trash: "<<trash<<"\n";
            if (name == "Coordinate") {
                SS>>trash>>R.CoordY;
                // std::cout<<"R.CoordY: "<<R.CoordY<<"\n";
                // system("pause");

            }
            else if (name == "Height")
                SS>>trash>>R.Height;
            else if (name == "Sitespacing")
                SS>>trash>>R.SiteSpacing;
            else if (name == "SubrowOrigin") {
                SS>>name>>R.CoordX>>trash>>trash>>R.NumSites;
            }
            else if (name == "End")
                break;
        }
        R.Width = R.NumSites*R.SiteSpacing;
        R.Xmax = R.CoordX + R.Width;
        // pm->InitRowVec.push_back(R);
        pm->InitRowVec[i] = R;
    }
    // pm->printRowVec();
}

void Parser::readNode() {
    std::string S, trash;
    for (int i=0; i<4; i++) 
        getline(nodeFile,S);
    {
        getline(nodeFile,S);
        std::stringstream SS(S);
        SS>>trash>>trash>>pm->NumNode;
    }
    {
        getline(nodeFile,S);
        std::stringstream SS(S);
        SS>>trash>>trash>>pm->NumTerminal;
        // getline(nodeFile,S);
    }
    // while (getline(nodeFile,S)) {
    for (int idx=0; idx<pm->NumNode; idx++) {
        getline(nodeFile,S);
        if (S.size() == 0)
            getline(nodeFile,S);
        std::string nodeN;
        std::stringstream SS(S);
        // int idx = pm->nodeNameToIdx[nodeN];
        SS>>nodeN>>pm->vertexVec[idx].Width>>pm->vertexVec[idx].Height;
        // if (idx == 1228259)
        //     pm->vertexVec[idx].printVertex();
        // std::cout<<"idx: "<<idx<<" Node name: "<<nodeN<<"\n";
        // if (idx >=211400) {
        //     std::cout<<idx<<" "<<pm->vertexVec[idx].Width<<" "<<pm->vertexVec[idx].Height<<"\n";
        // }
    }
    // pm->printVertexVec();
}
#endif