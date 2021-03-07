#ifndef _PARSER_H_
#define _PARSER_H_
#include <bits/stdc++.h>
// #include "Floorplan.h"
class Floorplan;
class Parser {
    public:
    std::ifstream inputBlock, inputNet;
    std::ofstream output;
    std::string BlockFileName, NetFileName, outputFileName;
    Floorplan* fp;
    Parser();
    Parser(Floorplan* f):fp(f) {}
    void readFile(const std::string, const std::string);
    void readBlock();
    void readNet();
    void writeOutput(const std::string, time_t);
};

void Parser::readFile (const std::string BlockFile, const std::string NetFile) {
    inputBlock.open(BlockFile);
    if (!inputBlock) {
        std::cerr << "Can't open "<<BlockFile<<"!\n";
        exit(1);    //if unusual, close it 
    }
    inputNet.open(NetFile);
    if (!inputNet) {
        std::cerr << "Can't open "<<NetFile<<"!\n";
        exit(1);    //if unusual, close it 
    }
    readBlock();
    readNet();
}

void Parser::readBlock() {
    std::string trash, block, terminal;
    int X, Y;
    inputBlock>>trash>>fp->outlineX>>fp->outlineY;
    inputBlock>>trash>>fp->NumBlock;
    inputBlock>>trash>>fp->NumTerminal;
    for (int i=0; i<fp->NumBlock; i++) {
        inputBlock>>block>>X>>Y;
        fp->blockNum[block] = i;
        fp->blockName[i] = block;
        fp->blockWH.push_back({X,Y});
    }
    int idx=fp->NumBlock;
    // fp->printBlock();
    for (int i=0; i<fp->NumTerminal; i++) {
        inputBlock>>terminal>>trash>>X>>Y;
        fp->terminalNum[terminal] = idx;
        fp->terminalName[idx] = terminal;
        fp->terminalXY.push_back({X,Y});
        idx++;
    }
    // fp->printTerminal();
}

void Parser::readNet() {
    std::string trash, block;
    int degree;
    inputNet>>trash>>fp->numNet;
    fp->netList.resize(fp->numNet);
    for (int i=0; i<fp->numNet; i++) {
        inputNet>>trash>>degree;
        for (int j=0; j<degree; j++) {
            inputNet>>block;
            if (fp->blockNum.find(block) != fp->blockNum.end())
                fp->netList[i].push_back(fp->blockNum[block]);
            else {
                fp->netList[i].push_back(fp->terminalNum[block]);
            }
                
        }
    }
    // fp->printNet();
}

void Parser::writeOutput(const std::string outputFile, time_t runtime) {
    outputFileName = outputFile;
    output.open(outputFileName);
    if (!output) {
        std::cerr<<"Can't open: "<<outputFile<<"!\n";
        exit(1);
    }
    output<<std::fixed<<std::setprecision(1)<<fp->cost<<"\n";
    output<<fp->W<<"\n"<<fp->totalBlockX*fp->totalBlockY<<"\n";
    output<<fp->totalBlockX<<" "<<fp->totalBlockY<<"\n";
    output<<runtime<<"\n";
    for (int i=0; i<fp->NumBlock; i++) {
        output<<fp->blockName[i]<<" "<<fp->blockXY[i].first<<" "<<fp->blockXY[i].second<<" "<<fp->blockXY[i].first + fp->blockWH[i].first<<" "<<fp->blockXY[i].second + fp->blockWH[i].second<<"\n";
        // std::cout<<fp->blockName[i]<<" "<<fp->blockCoordinate[i].first<<" "<<fp->blockCoordinate[i].second<<" "<<fp->blockCoordinate[i].first + fp->blockWH[i].first<<" "<<fp->blockCoordinate[i].second + fp->blockWH[i].second<<"\n";
    }
    output.close();
}

#endif