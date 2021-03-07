#ifndef _PARSER_H_
#define _PARSER_H_
#include <bits/stdc++.h>
#include "Router.h"
#include "Pin.h"
class Parser {
    public:
    Router* router;
    std::ifstream input;
    Parser();
    Parser(Router* r):router(r){}
    // Parser(Router*);
    void readInputFile(const std::string);
    void printChannel();
};

Parser::Parser(){}

void Parser::readInputFile(const std::string IFile) {
    input.open(IFile);
    if (!input) {
        std::cerr<<"Cannot open "<<IFile<<" !\n";
        exit(1);
    }
    std::string S;
    getline(input,S);
    {
        std::stringstream SS(S);
        int tmp, idx(0);
        while (SS>>tmp) {
            router->channel.push_back({tmp,0});
            if (!router->pin[tmp].appear) {
                router->pin[tmp].appear = true;
                router->pin[tmp].start = idx;
                router->pin[tmp].end = idx;
                router->pin[tmp].pos[idx] = true;
                router->pin[tmp].it=router->pin[tmp].pos.begin();
                // std::cout<<router->pin[tmp].it->first<<" "<<router->pin[tmp].it->second<<"\n";
            }
            else {
                if (router->pin[tmp].end < idx)
                    router->pin[tmp].end = idx;
                    
                if (router->pin[tmp].start > idx)
                    router->pin[tmp].start = idx;
                router->pin[tmp].pos[idx] = true;
            }
            idx++;
        }
    }
    getline(input,S);
    {
        std::stringstream SS(S);
        int tmp, idx(0);
        while (SS>>tmp) {
            router->channel[idx].second = tmp;
            if (!router->pin[tmp].appear) {
                router->pin[tmp].appear = true;
                router->pin[tmp].start = idx;
                router->pin[tmp].end = idx;
                router->pin[tmp].pos[idx] = false;
                router->pin[tmp].it=router->pin[tmp].pos.begin();
                // std::cout<<router->pin[tmp].it->first<<" "<<router->pin[tmp].it->second<<"\n";
            }
            else {
                if (router->pin[tmp].end < idx)
                    router->pin[tmp].end = idx;
                if (router->pin[tmp].start > idx)
                    router->pin[tmp].start = idx;
                router->pin[tmp].pos[idx] = false;
            }
            idx++;
        }
    }
    printChannel();
    router->printPin();
}

void Parser::printChannel() {
    for (int i=0; i<router->channel.size(); i++) {
        std::cout<<router->channel[i].first<<" ";
    }std::cout<<"\n";
    for (int i=0; i<router->channel.size(); i++)
        std::cout<<router->channel[i].second<<" ";
    std::cout<<"\n";
}
#endif