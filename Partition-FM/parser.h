#ifndef _PARSER_H_
#define _PARSER_H_
#include <bits/stdc++.h>
#include "vertex.h"
class Partition;
class Parser {
    public:
    int nodeNum;
    int netNum;
    Partition* partition;
    Parser();
    Parser(Partition *p): nodeNum(0), netNum(0), partition(p) {}
    void readFile(const std::string);
    void writeFile(std::string);
    void print_netList();
};
Parser::Parser():nodeNum(0), netNum(0){}

void Parser::readFile(const std::string inputFile) {
    std::ifstream input;
    std::string S;
    // std::cout<<"input file name "<<inputFile<<"\n";
    input.open(inputFile);
    if(!input) {    //test if file opened 
        std::cerr << "Can't open "<<inputFile<<"!\n";
        exit(1);     //if unusual, close it 
    }
    // std::cout<<input.peek()<<"\n";

    getline(input,S);
    {
        std::stringstream SS(S);
        SS>>netNum>>nodeNum;
    }
    partition->vertexVec.resize(nodeNum);
    partition->netList.resize(netNum);
    for (int i=0; i<netNum; i++) {
        getline(input,S);
        {
            std::stringstream SS(S);
            int tmp;
            while (SS>>tmp) {
                partition->netList[i].insert(tmp-1);
                partition->vertexVec[tmp-1].net.insert(i);
            }
        }
    }
}

void Parser::print_netList() {
    std::cout<<"Print Net List\n";
    for (auto it1=partition->netList.begin(); it1!=partition->netList.end(); it1++) {
        for (auto it2=it1->begin(); it2!=it1->end(); it2++) {
            std::cout<<*it2<<" ";
        }std::cout<<"\n";
    }
}
#endif