#ifndef _PIN_H_
#define _PIN_H_
#include <bits/stdc++.h>
class Pin {
    public:
    std::set<int> curTrack;
    bool appear;
    bool step3;
    bool completed;
    int start, end;
    std::map<int,bool>::iterator it;
    std::map<int,bool>  pos;//pos , top/bottom
    std::vector<std::tuple<int,int,int>> VPos;//(x,y,y)
    std::vector<std::tuple<int,int,int>> HPos;//(x,y,x)
    Pin():start(-1), end(-1), step3(false), appear(false), completed(false){}
    void printPinPos();
};

void Pin::printPinPos() {
    std::cout<<"start: "<<start<<" end: "<<end<<"\n";
}

#endif