#include <bits/stdc++.h>
#include "Placement.h"
#include "Parser.h"
using namespace std;
int main(int argc, char** argv) {
    if (argc<3) {
        cout<<"input format wrong!\nPlease enter: ./Lab4 [input.aux] [output.pl]\n";
        exit(1);
    }
    time_t start(time(NULL));
    Placement* pm = new Placement;
    Parser* parser = new Parser(pm);
    parser->readInput(argv[1]);
    // parser->readInput("adaptec1.aux");
    // pm->cutRow();
    // parser->readInput("newblue5.aux");
    pm->cutRow();
    pm->totalRow();
    pm->Abacus();
    // pm->outputGDT("adaptec1.gdt");
    // pm->outputGDT("newblue5.gdt");
    // cout<<"vertex number: "<<pm->vertexVec.size()<<"\n";
    // pm->OutputFile("output2.pl");
    pm->OutputFile(argv[2]);
    time_t end(time(NULL));
    std::cout<<"Cost Time: "<<end-start<<"\n";
}