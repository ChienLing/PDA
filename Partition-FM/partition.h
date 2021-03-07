#ifndef _PARTITION_H_
#define _PARTITION_H_
#include <bits/stdc++.h>
#include "vertex.h"
class Partition {
    public:
    int LBnum;
    int UBnum;
    int G1Size;
    int G0Size;
    int totalGain;
    int cutSize;
    std::vector<int> netDist;//record node in group 0;
    std::vector<vertex> vertexVec;
    std::vector<std::set<int>> netList;
    std::map<int,std::set<int>,std::greater<int>> G0gain;
    std::map<int,std::set<int>,std::greater<int>> G1gain;
    void fm();
    void FM();
    void calCutSize();
    void initial();
    Partition ();
    void resetMoved();
    void printGainMap();
    void print_NetDist();
    void UpdatingCellGain(bool G1, int nodeIdx);
    void printDistribution();
    void PrintNetList(int);
    void PrintMoved();
    void initialGain();

};
Partition::Partition ():LBnum(0),UBnum(0),G1Size(0),G0Size(0),totalGain(0){}

void Partition::initialGain () {
    for (int i=0; i<vertexVec.size(); i++) {
        vertexVec[i].TE = 0;
        vertexVec[i].FS = 0;
    }
    int totalG0node(0), totalG1node(0);
    for (int netIdx=0; netIdx<netList.size(); netIdx++) {
        int G0(0), G1(0);
        std::vector<int> G0List, G1List;
        for (auto netNode=netList[netIdx].begin(); netNode!=netList[netIdx].end(); netNode++) {
            if (!vertexVec[*netNode].group) {
                G0++;
                G0List.push_back(*netNode);
            }
            else {
                G1++;
                G1List.push_back(*netNode);
            }
        }
        netDist[netIdx] = G0;
        if (G0 == 1) {
            vertexVec[G0List[0]].FS++;
        }
        else if (G0 == 0) {
            for (int i=0; i<G1List.size(); i++) {
                vertexVec[G1List[i]].TE++;
            }
        }
        if (G1 == 1) {
            vertexVec[G1List[0]].FS++;
        }
        else if (G1 == 0) {
            for (int i=0; i<G0List.size(); i++)
                vertexVec[G0List[i]].TE++;
        }
    }
    for (int i=0; i<vertexVec.size(); i++) {
        vertexVec[i].gain = vertexVec[i].FS-vertexVec[i].TE;
        if (vertexVec[i].group) {
            G1gain[vertexVec[i].gain].insert(i);
            totalG1node++;
        }
        else{
            G0gain[vertexVec[i].gain].insert(i);
            totalG0node++;
        }
    }
    G0Size = totalG0node;
    G1Size = totalG1node;
    // std::cout<<"totalG0node: "<<totalG0node<<" totalG1node: "<<totalG1node<<"\n";
    // printGainMap();
}

void Partition::PrintMoved() {
    std::cout<<"PrintMoved\n";
    for (int i=0; i<vertexVec.size(); i++)
        std::cout<<vertexVec[i].moved<<" ";
    std::cout<<"\n";
}
void Partition::PrintNetList(int netIdx) {
    std::cout<<"PrintNetList\n";
    for (auto it=netList[netIdx].begin(); it!=netList[netIdx].begin(); it++)
        std::cout<<*it<<" ";
    std::cout<<"\n";
}
void Partition::calCutSize() {
    cutSize = 0;
    for (int i=0; i<netList.size(); i++) {
        bool cmp = vertexVec[*netList[i].begin()].group;
        for (auto nodeIdx=netList[i].begin(); nodeIdx!=netList[i].end(); nodeIdx++) {
            if (vertexVec[*nodeIdx].group != cmp){
                cutSize++;
                break;
            }
        }
    }
    std::cout<<"Cut Size: "<<cutSize<<"\n";
}

void Partition::initial() {
    int n=vertexVec.size();
    netDist.resize(netList.size());
    LBnum = n*0.45;
    UBnum = n*0.55;
    if (LBnum+UBnum < n)
        UBnum++;
    G1Size = UBnum;
    G0Size = n-UBnum;
    for (int i=0; i<UBnum; i++) {
        vertexVec[i].group = true;
    }
    initialGain();
}

void Partition::FM() {
    while(1) {
        fm();
        if (totalGain <= 0)
            break;
        else
        {
            totalGain=0;
        }
        
    }
}

void Partition::fm() {
    std::vector<std::pair<int,int>> moveOrder;
    int gain(0);
    int run(0);
    int i(0);
    for (i=0; i<vertexVec.size(); i++) {//change all the vertex
        int MaxGain=-1e8;
        bool G1(true);          //choosec vertex in group 1
        std::map<int,std::set<int>>::iterator eraseIt;
        int moveNodeIdx(0);
        int idx(0);
        if (G1Size > LBnum) {
            auto it1 = G1gain.begin();
            if (it1!=G1gain.end() && it1->first > MaxGain) {
                MaxGain = it1->first;
                moveNodeIdx = *it1->second.begin();
                eraseIt = it1;
            }
        }
        if (G0Size > LBnum) {
            auto it0 = G0gain.begin();
            if (it0!=G0gain.end() && it0->first > MaxGain) {
                G1=false;
                MaxGain = it0->first;
                moveNodeIdx = *it0->second.begin();
                eraseIt = it0;
            }
        }
        if (MaxGain == -1e8)
            std::cout<<"wrong\n";
        gain += MaxGain;
        if (totalGain < gain) {
            totalGain = gain;
            run = i;
        }
        if (G1) {
            G1Size -= 1;
            G0Size += 1;
            if (G1gain[MaxGain].size() == 1)
                G1gain.erase(MaxGain);
            else 
                G1gain[MaxGain].erase(moveNodeIdx);
            moveOrder.push_back({gain,moveNodeIdx});
            UpdatingCellGain (G1, moveNodeIdx);
        }
        else {
            G0Size -= 1;
            G1Size += 1;
            if (G0gain[MaxGain].size() == 1)
                G0gain.erase(MaxGain);
            else 
                G0gain[MaxGain].erase(moveNodeIdx);
            moveOrder.push_back({gain,moveNodeIdx});
            UpdatingCellGain (G1, moveNodeIdx);
        }
    }
    if (totalGain > 0) {
        for (int i=0; i<vertexVec.size(); i++) {
            vertexVec[i].moved = false;
        }
        for (int i=run+1; i<vertexVec.size(); i++) {
            vertexVec[moveOrder[i].second].group = !vertexVec[moveOrder[i].second].group;
        }
        initialGain();
    }
}

void Partition::UpdatingCellGain(bool G1, int nodeIdx) {
    // std::cout << nodeIdx << "\n";
    vertexVec[nodeIdx].group = !vertexVec[nodeIdx].group;
    vertexVec[nodeIdx].moved = true;
    // PrintMoved();
    if (G1) {
        for (auto netIdx=vertexVec[nodeIdx].net.begin(); netIdx!=vertexVec[nodeIdx].net.end(); netIdx++) {//all net connected with node Idx
            
            if (netDist[*netIdx] == 0) {
                for (auto nodeInNet=netList[*netIdx].begin(); nodeInNet!=netList[*netIdx].end(); nodeInNet++) {
                    int node = *nodeInNet;
                    int nodeInNetGain = vertexVec[*nodeInNet].gain;
                    if (!vertexVec[*nodeInNet].moved) {
                        if (vertexVec[*nodeInNet].group) {
                            if (G1gain[vertexVec[*nodeInNet].gain].size() == 1)
                                G1gain.erase(vertexVec[*nodeInNet].gain);
                            else
                                G1gain[vertexVec[*nodeInNet].gain].erase(*nodeInNet);
                            vertexVec[*nodeInNet].gain += 1;
                            G1gain[vertexVec[*nodeInNet].gain].insert(*nodeInNet);
                        }
                        else {
                            if (G0gain[vertexVec[*nodeInNet].gain].size() == 1)
                                G0gain.erase(vertexVec[*nodeInNet].gain);
                            else 
                                G0gain[vertexVec[*nodeInNet].gain].erase(*nodeInNet);
                            vertexVec[*nodeInNet].gain += 1;
                            G0gain[vertexVec[*nodeInNet].gain].insert(*nodeInNet);
                        }
                    }
                }
            }
            else if (netDist[*netIdx] == 1) {
                for (auto nodeInNet=netList[*netIdx].begin(); nodeInNet!=netList[*netIdx].end(); nodeInNet++) {
                    int node = *nodeInNet;
                    int nodeInNetGain = vertexVec[*nodeInNet].gain;
                    if (!vertexVec[*nodeInNet].group && !vertexVec[*nodeInNet].moved) {
                        if (G0gain[vertexVec[*nodeInNet].gain].size() == 1)
                            G0gain.erase(vertexVec[*nodeInNet].gain);
                        else 
                            G0gain[vertexVec[*nodeInNet].gain].erase(*nodeInNet);
                        vertexVec[*nodeInNet].gain -= 1;
                        G0gain[vertexVec[*nodeInNet].gain].insert(*nodeInNet);
                        break;
                    }
                }
            }
            netDist[*netIdx] += 1;
            if (netList[*netIdx].size()-netDist[*netIdx] == 0) {//number of node in net idx in Group 1
                for (auto nodeInNet=netList[*netIdx].begin(); nodeInNet!=netList[*netIdx].end(); nodeInNet++) {
                    int node = *nodeInNet;
                    int nodeInNetGain = vertexVec[*nodeInNet].gain;
                    if (!vertexVec[*nodeInNet].moved) {
                        if (vertexVec[*nodeInNet].group) {
                            if (G1gain[vertexVec[*nodeInNet].gain].size() == 1)
                                G1gain.erase(vertexVec[*nodeInNet].gain);
                            else 
                                G1gain[vertexVec[*nodeInNet].gain].erase(*nodeInNet);
                            vertexVec[*nodeInNet].gain -= 1;
                            G1gain[vertexVec[*nodeInNet].gain].insert(*nodeInNet);
                        }
                        else {
                            if (G0gain[vertexVec[*nodeInNet].gain].size() == 1)
                                G0gain.erase(vertexVec[*nodeInNet].gain);
                            else 
                                G0gain[vertexVec[*nodeInNet].gain].erase(*nodeInNet);
                            vertexVec[*nodeInNet].gain -= 1;
                            G0gain[vertexVec[*nodeInNet].gain].insert(*nodeInNet);
                        }
                    }
                }
            }
            else if (netList[*netIdx].size()-netDist[*netIdx] == 1) {
                for (auto nodeInNet=netList[*netIdx].begin(); nodeInNet!=netList[*netIdx].end(); nodeInNet++) {
                    int node = *nodeInNet;
                    int nodeInNetGain = vertexVec[*nodeInNet].gain;
                    if (vertexVec[*nodeInNet].group && !vertexVec[*nodeInNet].moved) {
                        if (G1gain[vertexVec[*nodeInNet].gain].size() == 1)
                            G1gain.erase(vertexVec[*nodeInNet].gain);
                        else 
                            G1gain[vertexVec[*nodeInNet].gain].erase(*nodeInNet);
                        vertexVec[*nodeInNet].gain += 1;
                        G1gain[vertexVec[*nodeInNet].gain].insert(*nodeInNet);
                        break;
                    }
                }
            }
        }
    }
    else {
        for (auto netIdx=vertexVec[nodeIdx].net.begin(); netIdx!=vertexVec[nodeIdx].net.end(); netIdx++) {
            if (netList[*netIdx].size()-netDist[*netIdx] == 0) {
                for (auto nodeInNet=netList[*netIdx].begin(); nodeInNet!=netList[*netIdx].end(); nodeInNet++) {
                    int node = *nodeInNet;
                    int nodeInNetGain = vertexVec[*nodeInNet].gain;
                    if (!vertexVec[*nodeInNet].moved) {
                        if (vertexVec[*nodeInNet].group) {
                            if (G1gain[vertexVec[*nodeInNet].gain].size() == 1)
                                G1gain.erase(vertexVec[*nodeInNet].gain);
                            else 
                                G1gain[vertexVec[*nodeInNet].gain].erase(*nodeInNet);
                            vertexVec[*nodeInNet].gain += 1;
                            G1gain[vertexVec[*nodeInNet].gain].insert(*nodeInNet);
                        }
                        else {
                            if (G0gain[vertexVec[*nodeInNet].gain].size() == 1)
                                G0gain.erase(vertexVec[*nodeInNet].gain);
                            else 
                                G0gain[vertexVec[*nodeInNet].gain].erase(*nodeInNet);
                            vertexVec[*nodeInNet].gain += 1;
                            G0gain[vertexVec[*nodeInNet].gain].insert(*nodeInNet);
                        }
                    }
                }
            }
            else if (netList[*netIdx].size()-netDist[*netIdx] == 1) {
                for (auto nodeInNet=netList[*netIdx].begin(); nodeInNet!=netList[*netIdx].end(); nodeInNet++) {
                    int node = *nodeInNet;
                    int nodeInNetGain = vertexVec[*nodeInNet].gain;
                    if (vertexVec[*nodeInNet].group && !vertexVec[*nodeInNet].moved) {
                        if (G1gain[vertexVec[*nodeInNet].gain].size() == 1)
                            G1gain.erase(vertexVec[*nodeInNet].gain);
                        else 
                            G1gain[vertexVec[*nodeInNet].gain].erase(*nodeInNet);
                        vertexVec[*nodeInNet].gain -= 1;
                        G1gain[vertexVec[*nodeInNet].gain].insert(*nodeInNet);
                        break;
                    }
                }
            }
            netDist[*netIdx] -= 1;
            if (netDist[*netIdx] == 0) {
                for (auto nodeInNet=netList[*netIdx].begin(); nodeInNet!=netList[*netIdx].end(); nodeInNet++) {
                    int node = *nodeInNet;
                    int nodeInNetGain = vertexVec[*nodeInNet].gain;
                    if (!vertexVec[*nodeInNet].moved) {
                        if (vertexVec[*nodeInNet].group) {
                            if (G1gain[vertexVec[*nodeInNet].gain].size() == 1)
                                G1gain.erase(vertexVec[*nodeInNet].gain);
                            else 
                                G1gain[vertexVec[*nodeInNet].gain].erase(*nodeInNet);
                            vertexVec[*nodeInNet].gain -= 1;
                            G1gain[vertexVec[*nodeInNet].gain].insert(*nodeInNet);
                        }
                        else {
                            if (G0gain[vertexVec[*nodeInNet].gain].size() == 1)
                                G0gain.erase(vertexVec[*nodeInNet].gain);
                            else 
                                G0gain[vertexVec[*nodeInNet].gain].erase(*nodeInNet);
                            vertexVec[*nodeInNet].gain -= 1;
                            G0gain[vertexVec[*nodeInNet].gain].insert(*nodeInNet);
                        }
                    }
                }
            }
            else if (netDist[*netIdx] == 1) {
                for (auto nodeInNet=netList[*netIdx].begin(); nodeInNet!=netList[*netIdx].end(); nodeInNet++) {
                    int node = *nodeInNet;
                    int nodeInNetGain = vertexVec[*nodeInNet].gain;
                    if (!vertexVec[*nodeInNet].group && !vertexVec[*nodeInNet].moved) {
                        if (G0gain[vertexVec[*nodeInNet].gain].size() == 1)
                            G0gain.erase(vertexVec[*nodeInNet].gain);
                        else 
                            G0gain[vertexVec[*nodeInNet].gain].erase(*nodeInNet);
                        vertexVec[*nodeInNet].gain += 1;
                        G0gain[vertexVec[*nodeInNet].gain].insert(*nodeInNet);
                    }
                }
            }
        }
    }
}

void Partition::resetMoved() {
    for (int idx=0; idx<vertexVec.size(); idx++)
        vertexVec[idx].moved = false;
}

void Partition::printGainMap() {
    std::cout<<"Print Gain Map\nGroup 0:\n";
    for (auto it=G0gain.begin(); it!=G0gain.end(); it++) {
        std::cout<<"gain "<<it->first<<"\n";
        for (auto it2=it->second.begin(); it2!=it->second.end(); it2++) {
            std::cout<<*it2<<" ";
        }
        std::cout<<"\n";
    }
    std::cout<<"Group 1:\n";
    for (auto it=G1gain.begin(); it!=G1gain.end(); it++) {
        std::cout<<"gain "<<it->first<<"\n";
        for (auto it2=it->second.begin(); it2!=it->second.end(); it2++) {
            std::cout<<*it2<<" ";
        }
        std::cout<<"\n";
    }
}

void Partition::print_NetDist() {
    std::cout<<"Print NetDist:\n";
    for (int idx=0; idx<netDist.size(); idx++) {
        std::cout<<"net "<<idx<<" G0: "<<netDist[idx]<<" G1: "<<netList[idx].size()-netDist[idx]<<"\n";
    }
}

void Partition::printDistribution() {
    std::cout<<"Node Distribution \nGroup 0: \n";
    std::vector<int> vec;
    for (int i=0; i<vertexVec.size(); i++) {
        if (!vertexVec[i].group)
            std::cout<<i<<" ";
        else
            vec.push_back(i);
    }
    std::cout<<"\nGroup 1:\n";
    for (int i=0; i<vec.size(); i++)
        std::cout<<vec[i]<<" ";
    std::cout<<"\n";
}
#endif