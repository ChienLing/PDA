#include <bits/stdc++.h>
#include "parser.h"
#include "Router.h"

using namespace std;

int main(int argc, char**argv) {
    Router* router = new Router;
    Parser* parser = new Parser(router);
    parser->readInputFile("case 2.txt");
    router->TrackNum = router->pin.size()/2;
    router->Greedy();
    router->splitover();
    router->channelSize++;
    router->channel.resize(router->channelSize,{0,0});
    router->trackAssign.resize(router->channelSize);
    router->trackAssign[router->channelSize-1].resize(router->TrackNum);
    router->VerticleWire.resize(router->channelSize);
    router->VerticleWire[router->channelSize-1].resize(router->TrackNum,-1);
    router->c++;
    router->initialTraclAssign();
    router->printChannelH();
    router->printChannelV();
    router->outputGDT("output2.gdt");
    return 0;
}