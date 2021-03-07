#include <bits/stdc++.h>
#include "Floorplan.h"
#include "Parser.h"
using namespace std;

int main(int argc, char** argv) {
    time_t start(time(0));
    Floorplan* FP = new Floorplan;
    Parser *P = new Parser(FP);
    
    std::vector<std::pair<int,int>> CoordinateVec;
    std::vector<std::pair<int,int>> WidthHeightVec;
    std::vector<int> Gamma1;
    std::vector<int> Gamma2;
    
    // P->readFile("testblock.txt", "testnet.txt");
    // P->readFile("ami33.block", "ami33.nets");
    // P->readFile("ami49.block", "ami49.nets");
    P->readFile(argv[2], argv[3]);
    std::srand ( unsigned (std::time(0)));
    
    for (int i=0; i<FP->NumBlock; i++) {
        FP->Gamma1.push_back(i);
        FP->Gamma2.push_back(i);
    }
    string S(argv[1]);
    stringstream SS(S);
    SS>>FP->alpha;
    std::cout<<FP->alpha<<"\n";
    FP->alpha = 0.5;
    FP->blockXY.resize(FP->NumBlock);

    pair<int,int> WidthHeight={1e6, 1e6};
    long long area = FP->getArea(WidthHeight.first,WidthHeight.second);
    // std::random_shuffle(FP->Gamma1.begin(), FP->Gamma1.end());      std::random_shuffle(FP->Gamma2.begin(), FP->Gamma2.end());

    for (int i=0; i<700; i++) {
        std::random_shuffle(FP->Gamma1.begin(), FP->Gamma1.end());      std::random_shuffle(FP->Gamma2.begin(), FP->Gamma2.end());
        FP->T = 1e10;
        FP->SA();
        if (FP->getArea(FP->totalBlockX ,FP->totalBlockY) < area) {
            WidthHeight = {FP->totalBlockX ,FP->totalBlockY};
            area = FP->getArea(FP->totalBlockX ,FP->totalBlockY);
            CoordinateVec = FP->blockXY;
            WidthHeightVec = FP->bestWH;
            std::cout<<"Width: "<<WidthHeight.first<<" Height: "<<WidthHeight.second<<"\n";
        }
        if (WidthHeight.first<=FP->outlineX && WidthHeight.second<=FP->outlineY)
            break;
    }
    FP->blockXY = CoordinateVec;
    FP->blockWH = WidthHeightVec;
    FP->totalBlockX = WidthHeight.first;
    FP->totalBlockY = WidthHeight.second;
    std::cout<<"Width: "<<WidthHeight.first<<" Height: "<<WidthHeight.second<<"\n";
    // FP->outputGDT("ami33.gdt");
    // FP->outputGDT("ami49.gdt");
    // FP->outputGDT("testblock.gdt");
    FP->getW(); std::cout<<"HPWL: "<<FP->W<<"\n";
    FP->getCost();  std::cout<<"Cost: "<<FP->cost<<"\n";
    time_t end(time(0));
    std::cout<<"time cost(s): "<<start-end;
    // P->writeOutput(argv[4], start-end);
    std::cout<<"block num: "<<FP->NumBlock<<"\n";
    
    // P->writeOutput("output.rpt", end-start, HPWL, cost);
    P->writeOutput(argv[4], end-start);

    // cout<<"result: "<<FP->totalBlockX<<" "<<FP->totalBlockY<<"\n";
    // cout<<"Result: "<<FP->totalBlockX<<" "<<FP->totalBlockY<<"\n";
}