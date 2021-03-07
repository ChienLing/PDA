#include <bits/stdc++.h>
#include "partition.h"
#include "parser.h"
using namespace std;
int main(int argc, char **argv) {
    if (argc < 2) {
    cout<<"Please input file!!\n"; 
    }
    Partition* partition = new Partition;
    Parser* parser = new Parser(partition);
    // parser->readFile("input2.txt");
    parser->readFile(argv[1]);

    
    partition->initial();
    // partition->calCutSize();
    partition->initialGain();
    partition->FM();
    // partition->calCutSize();
    
    ofstream output("output.txt");
    for (int i=0; i<partition->vertexVec.size(); i++) 
        output<<(int)partition->vertexVec[i].group<<"\n";
    return 0;
}