#ifndef _ROUTER_H_
#define _ROUTER_H_
#define T first 
#define B second 

#include <bits/stdc++.h>
#include "Pin.h"
class Router {
    public:
    int c, channelSize;
    int TrackNum;
    bool connectT, connectB;
    int T_E_Track, B_E_Track;//top empty track, bottom empty track
    std::set<int> endPin;
    std::map<int,int> countTrackPin;
    std::map<int,Pin> pin;
    std::vector<bool> emptyTrack;
    std::vector<bool> VWire;//true : have wire, false : no virtical wire
    std::vector<std::pair<int,int>> channel;//T, B
    std::vector<std::vector<std::pair<int,int>>> trackAssign;//track, net
    std::vector<std::vector<int>> VerticleWire;//track, net
    Router();
    void Greedy();
    void step1();
    void step2();
    void step3();
    void step4();
    void step5();
    void step6();
    void splitover();
    void resetStep3();
    void reorderTrack(int);
    int FindTETrack();
    int FindBETrack();
    int FindVerTrack(int,int,int);
    void printPin();
    void fillVWire(int, int);
    void fillVerticleWire(int, int, int, int);
    bool testMerge(int, int, int);
    int farthestTrack(int, bool);
    bool testSplitover();
    void initialTraclAssign();
    void outputGDT(const std::string);
    void printTrackAssign();
    void printTrackAssign(int);
    void printChannelSize();
    void printChannelH();
    void printChannelV();
    void printChannelV(int);
    void testPinTrack();
    void printHPos(std::map<int,std::map<int,int>>& );
    std::map<int,std::vector<int>> findMostSplitNet();
};

Router::Router(){}

void Router::Greedy() {
    c=0;
    channelSize = channel.size();
    trackAssign.resize(channel.size());
    VerticleWire.resize(channel.size());
    emptyTrack.resize(TrackNum, true);
    initialTraclAssign();
    for (int i=0; i<channel.size(); i++) {
        c=i;
        
        trackAssign[c].resize(TrackNum);
        VerticleWire[c].resize(TrackNum,-1);
        step1();
        // std::cout<<"step 1\n";
        // printTrackAssign();
        // printChannelV(c);
        // printChannelSize();
        testPinTrack();
        step2();
        // std::cout<<"step 2\n";
        // printTrackAssign();
        // printChannelV(c);
        // printChannelSize();
        testPinTrack();
        step3();
        // std::cout<<"step 3\n";
        // printTrackAssign();
        // printChannelV(c);
        // printChannelSize();
        testPinTrack();
        step4();
        // std::cout<<"step 4\n";
        // printTrackAssign();
        // printChannelV(c);
        // printChannelSize();
        testPinTrack();
        step5();
        // std::cout<<"step 5\n";
        // printTrackAssign();
        // printChannelV(c);
        // printChannelSize();
        testPinTrack();
        step6();
        // std::cout<<"step 6\n";
        // printTrackAssign();
        // printChannelV(c);
        // printChannelSize();
        testPinTrack();
    }

}

void Router::step1() {
    int Ti(channel[c].T), Bi(channel[c].B);
    T_E_Track = FindTETrack();
    B_E_Track = FindBETrack();
    if (Ti != 0)
        pin[Ti].it++;
    if (Bi != 0)
        pin[Bi].it++;
    if (pin[Ti].end == c && Ti!=0 ) {
        endPin.insert(Ti);
    }
    if (pin[Bi].end == c && Bi!=0) {
        endPin.insert(Bi);
    }
    int A_TopTrack, A_BotTrack;//Assigned Top Track   Assigned Bottom Track
    VWire.clear();
    if ((T_E_Track == -1 && pin[Ti].curTrack.empty()) && (B_E_Track == -1 && pin[Bi].curTrack.empty()) ) {//figure 4  no empty track 
        // if (pin[Ti].curTrack)
        connectT = false;
        connectB = false;
        if (Ti == Bi && Ti!=0) {  //Figure 6 alought no empty track, but Ti = Bi, then connect them
            VWire.resize(TrackNum, true);
            fillVerticleWire(0, TrackNum-1, c, Ti);
            return;
        }
        VWire.resize(TrackNum, false);
        return;
    }
    connectT = true;
    connectB = true;
    // if ((T_E_Track == -1 && pin[Ti].curTrack.empty()))
    //     connectT = false;
    // else if ((B_E_Track == -1 && pin[Bi].curTrack.empty()))
    //     connectB = false;
    VWire.resize(TrackNum, false);
    int VWireSize = VWire.size();
    ///////////////////            treat top pin Ti              ///////////////////
    if (Ti!=0 && !pin[Ti].curTrack.empty() && (*pin[Ti].curTrack.begin() > T_E_Track || T_E_Track == INT_MAX)) {
        A_TopTrack = *pin[Ti].curTrack.begin();
    }
    else if (Ti!=0){
        if (T_E_Track == INT_MAX)  
            connectT = false;
        A_TopTrack = T_E_Track;
    }
    
    ///////////////////            treat bottom pin Ti              ///////////////////
    if (Bi!=0 && !pin[Bi].curTrack.empty() && (*pin[Bi].curTrack.rbegin() < B_E_Track || B_E_Track == -1)) {
        A_BotTrack = *pin[Bi].curTrack.rbegin();
    }
    else if (Bi!=0) {
        if (B_E_Track == -1)  
            connectB = false;
        A_BotTrack = B_E_Track;
    }

    if (A_TopTrack <= A_BotTrack) {//two net overlap
        if (A_TopTrack == A_BotTrack ) {
            if (Ti != Bi) {
                if (A_BotTrack >= TrackNum/2) //close to top
                    connectB=false;
                else
                    connectT=false;
            }
        }
        else {
            if (TrackNum-A_TopTrack-1 < A_BotTrack) //assign for top edge
                connectB=false;
            else
                connectT=false;
        }
    }
    
    if (connectT && Ti!=0) {
        if (emptyTrack[A_TopTrack]) {
            countTrackPin[Ti]++;
            trackAssign[c][A_TopTrack] = {A_TopTrack,Ti};
            emptyTrack[A_TopTrack] = false;
            pin[Ti].curTrack.insert(A_TopTrack);
        }
        fillVerticleWire(A_TopTrack, TrackNum-1, c, Ti);
        fillVWire(A_TopTrack, TrackNum-1);
    }
    if (connectB && Bi!=0) {
        if (emptyTrack[A_BotTrack]) {
            countTrackPin[Bi]++;
            trackAssign[c][A_BotTrack] = {A_BotTrack, Bi};
            emptyTrack[A_BotTrack] = false;
            pin[Bi].curTrack.insert(A_BotTrack);
        }
        fillVerticleWire(0, A_BotTrack, c, Bi);
        fillVWire(0, A_BotTrack);
    }
    return;
}

void Router::step2() {
    int Ti = channel[c].T;
    int Bi = channel[c].B;
    std::vector<int> vec;
    for (auto p=endPin.begin(); p!=endPin.end(); p++) {
        int i=*p;
        vec.push_back(*p); 
    }

    for (auto p=vec.begin(); p!=vec.end(); p++) {
        int topT = *pin[*p].curTrack.begin();
        int botT = *pin[*p].curTrack.rbegin();
        bool merge = testMerge(*pin[*p].curTrack.begin(), *pin[*p].curTrack.rbegin(), *p);
        if (merge) {
            fillVWire(*pin[*p].curTrack.begin(), *pin[*p].curTrack.rbegin());
            fillVerticleWire(*pin[*p].curTrack.begin(), *pin[*p].curTrack.rbegin(), c, *p);
            countTrackPin.erase(*p);
            endPin.erase(*p);
            pin[*p].completed = true;
            
            for (auto pinTrack=pin[*p].curTrack.begin(); pinTrack!=pin[*p].curTrack.end(); pinTrack++) {
                if (c>0 && c<trackAssign.size() ) {
                    if (trackAssign[c-1][*pinTrack].second != *p )//&& trackAssign[c+1][*pinTrack] != *p
                        trackAssign[c][*pinTrack].second = -1;
                }   
                emptyTrack[*pinTrack] = true;
            }
            pin[*p].curTrack.clear();
        }
    }
    std::map<int,std::vector<int>> MPin = findMostSplitNet();
    for (auto it=MPin.rbegin(); it!=MPin.rend(); it++) {
        int Tnum = it->first;
        if (it->first <= 1)
            break;
        for (auto p=it->second.begin(); p!=it->second.end(); p++) {
            int P = *p;
            std::set<int> tmpTrack = pin[*p].curTrack;
            bool merge = testMerge(*pin[*p].curTrack.begin(), *pin[*p].curTrack.rbegin(), *p);
            if (merge) {
                countTrackPin[*p] -= pin[*p].curTrack.size()-1;
                fillVWire (*pin[*p].curTrack.begin(), *pin[*p].curTrack.rbegin());
                fillVerticleWire(*pin[*p].curTrack.begin(), *pin[*p].curTrack.rbegin(), c, *p);
                int remainTrack;
                if (pin[*p].it->second) {
                    remainTrack = *pin[*p].curTrack.rbegin();
                    auto a=pin[*p].curTrack.rbegin(); a++;
                    for (auto t=a; t!=pin[*p].curTrack.rend(); t++) {
                        // trackAssign[c][*t].second = -1;
                        if (c>0&&c<trackAssign.size()) {
                            if (trackAssign[c-1][*t].second!=*p )//&& trackAssign[c+1][*t].second!=*p , now we dont have trackAssign[c+1]
                                trackAssign[c][*t].second = -1;
                        }
                        emptyTrack[*t] = true;
                    }
                }
                else {
                    remainTrack = *pin[*p].curTrack.begin();
                    auto a=pin[*p].curTrack.begin(); a++;
                    for (auto t=a; t!=pin[*p].curTrack.end(); t++) {
                        // trackAssign[c][*t].second = -1;
                        if (c>0&&c<trackAssign.size()) {
                            if (trackAssign[c-1][*t].second!=*p )//&& trackAssign[c+1][*t].second!=*p , now we dont have trackAssign[c+1]
                                trackAssign[c][*t].second = -1;
                        }
                        emptyTrack[*t] = true;
                    }
                }
                pin[*p].curTrack.clear();
                pin[*p].curTrack.insert(remainTrack);
            }
        }
    }
}

void Router::step3() {
    for (auto t=trackAssign[c].begin();t!=trackAssign[c].end();t++) {
        int p=t->second;
        if (countTrackPin[p] > 1 && !pin[p].step3) {//pin.step3 means whether this pin had been shrinked. 
            pin[p].step3 = true;
            int topT = farthestTrack(*pin[p].curTrack.rbegin(),0);//find downward
            int botT = farthestTrack(*pin[p].curTrack.begin(),1);//find upward
            if (topT != *pin[p].curTrack.rbegin()) {
                fillVWire(topT, *pin[p].curTrack.rbegin());
                fillVerticleWire(topT, *pin[p].curTrack.rbegin(),c,p);
                emptyTrack[topT] = false;
                emptyTrack[*pin[p].curTrack.rbegin()] = true;
                if (c>0 && trackAssign[c-1][*pin[p].curTrack.rbegin()].second!=p)
                    trackAssign[c][*pin[p].curTrack.rbegin()].second = -1;
                trackAssign[c][topT].second = p;
                pin[p].curTrack.erase(*pin[p].curTrack.rbegin());
                pin[p].curTrack.insert(topT);
            }
            if (botT != *pin[p].curTrack.begin()) {
                fillVWire(*pin[p].curTrack.begin(), botT);
                fillVerticleWire(*pin[p].curTrack.begin(), botT,c,p);
                emptyTrack[botT] = false;
                emptyTrack[*pin[p].curTrack.begin()] = true;
                if (c>0 && trackAssign[c-1][*pin[p].curTrack.begin()].second!=p)
                    trackAssign[c][*pin[p].curTrack.begin()].second = -1;
                trackAssign[c][botT].second = p;
                pin[p].curTrack.erase(*pin[p].curTrack.begin());
                pin[p].curTrack.insert(botT);
            }
        }
    }
    resetStep3();
}

void Router::step4() {
    for (auto t=trackAssign[c].begin(); t!=trackAssign[c].end(); t++) {
        if (t->second == -1)//emptyTrack[t->first]
            continue;
        int track = t->first; 
        int p = t->second;
        if (p==-1) 
            std::cout<<"step4: track "<<track<< " not assign any net!\n";
        int tmp = pin[p].curTrack.size();
        if (pin[p].curTrack.find(t->first)!=pin[p].curTrack.end()) { //this net hadn't be merged
            int pos = pin[p].it->first;
            bool ori = pin[p].it->second;
            int shiftTrack = farthestTrack(t->first, pin[p].it->second);
            if (t->first != shiftTrack) {
                if (pin[p].it->second) {//rising
                    fillVWire(t->first, shiftTrack);
                    fillVerticleWire(t->first, shiftTrack,c,p);
                }
                else {
                    fillVWire(shiftTrack, t->first);
                    fillVerticleWire(shiftTrack, t->first,c,p);
                }
                trackAssign[c][t->first].second = -1;
                pin[p].curTrack.erase(t->first);
                if (!emptyTrack[shiftTrack])
                    std::cout<<"step 4 error!\n";
                trackAssign[c][t->second].second = p;
                pin[p].curTrack.insert(shiftTrack);
                emptyTrack[t->first] = true;
                emptyTrack[shiftTrack] = false;
            }
        }
    }
}

void Router::step5() {
    int insertTopTrack(TrackNum/2);
    int Ti = channel[c].T;
    int Bi = channel[c].B;
    if (!connectT && Ti!=0) {
        for (int i=TrackNum-1; i>=TrackNum/2; i--) {
            if (VWire[i] == true) {
                insertTopTrack = i+1;
                break;
            }
        }
        TrackNum++;
        trackAssign[c].resize(TrackNum,{TrackNum-1,-1});
        VerticleWire[c].resize(TrackNum,-1);
        trackAssign[c][TrackNum-1].first = TrackNum-1;
        emptyTrack.resize(TrackNum);
        VWire.resize(TrackNum);
        for (int i=TrackNum-1; i>insertTopTrack; i--) {
            int p = trackAssign[c][i-1].second;
            trackAssign[c][i].second = p;
            VWire[i] = VWire[i-1];//redundant
            VerticleWire[c][i] = VerticleWire[c][i-1];
            emptyTrack[i] = emptyTrack[i-1];
            if (p!=-1 && !emptyTrack[i]) {
                pin[p].curTrack.erase(i-1);
                pin[p].curTrack.insert(i);
            }
        }
        trackAssign[c][insertTopTrack].second = channel[c].T;
        pin[channel[c].T].curTrack.insert(insertTopTrack);
        emptyTrack[insertTopTrack] = false;
        fillVWire(insertTopTrack,TrackNum-1);
        fillVerticleWire(insertTopTrack,TrackNum-1,c,Ti);
        // printTrackAssign();
        countTrackPin[channel[c].T]++;

        for (int C=0; C<c; C++) {
            int TrackSize = trackAssign[C].size();
            trackAssign[C].resize(TrackNum,{TrackNum-1,-1});
            trackAssign[C][TrackNum-1].first = TrackNum-1;      //trackAssign[C][TrackNum-1].second = -1;
            VerticleWire[C].resize(TrackNum,-1);
            for (int i=TrackNum-1; i>insertTopTrack; i--) {
                trackAssign[C][i].second = trackAssign[C][i-1].second;
                VerticleWire[C][i] = VerticleWire[C][i-1];
            }
            trackAssign[C][insertTopTrack].second = -1;
            VerticleWire[C][insertTopTrack] = -1;
            printChannelV(c);////////////////////////////////////////////////////
            if (insertTopTrack<TrackNum-1 && insertTopTrack>0) {
                if (VerticleWire[C][insertTopTrack-1] == VerticleWire[C][insertTopTrack+1])
                    VerticleWire[C][insertTopTrack] = VerticleWire[C][insertTopTrack-1];
            }
        }
        printChannelV(c);
    }
    int insertBotTrack(TrackNum/2);
    if (!connectB && Ti!=Bi && Bi!=0) {
        for (int i=0; i<=TrackNum/2; i++) {
            if (VWire[i] == true) {
                insertBotTrack = i;
                break;
            }
        }
        TrackNum++;
        trackAssign[c].resize(TrackNum,{TrackNum-1,-1});
        trackAssign[c][TrackNum-1].first = TrackNum-1;
        emptyTrack.resize(TrackNum);
        VWire.resize(TrackNum);
        VerticleWire[c].resize(TrackNum);
        for (int i=TrackNum-1; i>insertBotTrack; i--) {
            int p = trackAssign[c][i-1].second;
            trackAssign[c][i].second = p;
            VWire[i] = VWire[i-1];//redundant
            VerticleWire[c][i] = VerticleWire[c][i-1];
            emptyTrack[i] = emptyTrack[i-1];
            if (p!=-1 && !emptyTrack[i]) {
                pin[p].curTrack.erase(i-1);
                pin[p].curTrack.insert(i);
            }
        }
        // printTrackAssign();
        fillVWire(0,insertBotTrack); 
        fillVerticleWire(0,insertBotTrack,c,Bi);
        trackAssign[c][insertBotTrack].second = channel[c].B;
        pin[channel[c].B].curTrack.insert(insertBotTrack);
        emptyTrack[insertBotTrack] = false;
        countTrackPin[channel[c].B]++;

        for (int C=0; C<c; C++) {
            int i=trackAssign[C].size();
            trackAssign[C].resize(TrackNum,{TrackNum-1,-1});
            trackAssign[C][TrackNum-1].first = TrackNum-1;
            VerticleWire[C].resize(TrackNum,-1);
            for (int i=TrackNum-1; i>insertBotTrack; i--) {
                trackAssign[C][i].second = trackAssign[C][i-1].second;
                VerticleWire[C][i] = VerticleWire[C][i-1];
            }
            trackAssign[C][insertBotTrack].second = -1;
            if (insertBotTrack<TrackNum-1 && insertBotTrack>0) {
                if (VerticleWire[C][insertBotTrack-1] == VerticleWire[C][insertBotTrack+1])
                    VerticleWire[C][insertBotTrack] = VerticleWire[C][insertBotTrack-1];
            }
            else 
                VerticleWire[C][insertBotTrack] = -1;

        }
        printChannelV(c);
    }
}

void Router::step6() {
    int CSize = channel.size();
    if (c >= channel.size()-1)
        return;
    trackAssign[c+1].resize(TrackNum);
    for (int i=0; i<TrackNum; i++) {
        if (!emptyTrack[i]) {
            trackAssign[c+1][i].first = trackAssign[c][i].first;
            trackAssign[c+1][i].second = trackAssign[c][i].second;
        }
        else {
            trackAssign[c+1][i].first = trackAssign[c][i].first;
            trackAssign[c+1][i].second = -1;
        }
            
    }
}

void Router::splitover() {
    while (testSplitover()) {
        channelSize++;
        channel.resize(channelSize,{0,0});
        trackAssign.resize(channelSize);
        VerticleWire.resize(channelSize);
        trackAssign[channelSize-1].resize(TrackNum);//??????????????????????????????step 6 have this
        VerticleWire[channelSize-1].resize(TrackNum,-1);
        VWire.clear();
        VWire.resize(TrackNum,false);
        // trackAssign[c]
        step6();
        c++;
        printTrackAssign();
        printChannelV(c);
        step2();
        printTrackAssign();
        printChannelV(c);
        step3();
        printTrackAssign();
        printChannelV(c);
        step4();
        printTrackAssign();
        printChannelV(c);
    }
    
}

bool Router::testSplitover() {
    for (auto p=pin.begin(); p!=pin.end(); p++) {
        if (!p->second.curTrack.empty())
            return true;
    }
    return false;
}

std::map<int,std::vector<int>> Router::findMostSplitNet() {
    std::map<int,std::vector<int>> ans;
    for (auto it=countTrackPin.begin(); it!=countTrackPin.end(); it++) {
        ans[it->second].push_back(it->first);
    }
    return ans;
}

void Router::fillVWire(int bot, int top) {
    for (int i=bot; i<=top; i++) {
        VWire[i] = true;
    }
}

void Router::fillVerticleWire(int bot, int top, int cha, int p) {
    for (int i=bot; i<=top; i++) {
        VerticleWire[cha][i] = p;
    }
}

void Router::resetStep3() {
    for (auto i=pin.begin(); i!=pin.end(); i++)
        i->second.step3 = false;
    return;
}

bool Router::testMerge(int bot, int top, int p) {
    for (int i=bot; i<=top; i++) {
        if (VWire[i] && trackAssign[c][i].second != p)//&& trackAssign[c][i].second == -1 ???????????????????????????
            return false;
    }
    return true;
}

int Router::FindBETrack() {
    for (int i=0; i<emptyTrack.size(); i++) {
        if (emptyTrack[i])
            return i;
    }
    return -1;
}

int Router::FindTETrack() {
    for (int i=emptyTrack.size(); i>0; i--) {
        if (emptyTrack[i-1])
            return i-1;
    }
    return INT_MAX;
}

int Router::FindVerTrack(int start, int p, int cha) {
    while(start<TrackNum && VerticleWire[cha][start] == p)   
        start++;
    return --start;
}

void Router::initialTraclAssign () {
    trackAssign[c].resize(TrackNum);
    for (int i=0; i<TrackNum; i++) {
        trackAssign[c][i] = {i,-1};
    }
}

int Router::farthestTrack(int start, bool r) {
    int FTrack(start);
    if (r) {
        while (FTrack+1 < TrackNum && emptyTrack[FTrack+1] && !VWire[FTrack+1])
            FTrack++;
    }
    else {
        while (FTrack-1 >=0 && emptyTrack[FTrack-1] && !VWire[FTrack-1])
            FTrack--;
    }
    return FTrack;
}

void Router::printPin() {
    for (auto it=pin.begin(); it!=pin.end(); it++) {
        std::cout<<it->first<<" ";it->second.printPinPos();
    }
}

void Router::outputGDT(const std::string outputFileName) {
    int L(0), H(TrackNum+1);
    int Ti(channel[0].T), Bi(channel[0].B);
    std::set<int> VConnect;
    for (auto p=pin.begin(); p!=pin.end(); p++) {
        if (!p->second.curTrack.empty())
            std::cout<<"pin "<<p->first<<" didnt complete!\n";
        p->second.curTrack.clear();
    }
    std::map<int,std::map<int,int>> HPos;//pin, track, pos
    std::map<int,std::map<int,int>> VPos;//pin, track, pos
    std::ofstream gdt(outputFileName);
    if (!gdt) {
        std::cerr<<"cannot open "<<outputFileName<<"!\n";
        exit(1);
    }
    gdt<<"gds2{600\n"
       <<"m=2018-09-14 14:26:15 a=2018-09-14 14:26:15\n"
       <<"lib 'asap7sc7p5t_24_SL' 0.00025 2.5e-10\n"
       <<"cell{c=2018-09-14 14:26:15 m=2018-09-14 14:26:15 'AND2x2_ASAP7_75t_SL'\n";

    if (channel[0].T == channel[0].B) {
        gdt<<"b{"<<channel[0].T<<" xy("<<0<<" "<<H<<" "<<0<<" "<<L<<" "<<0.1<<" "<<L<<" "<<0.1<<H<<") }\n";
        int TopTrack;
        for (auto t=trackAssign[0].begin(); t!=trackAssign[0].end(); t++) {
            if (t->second == channel[0].T) {
                TopTrack = t->first;
                break;
            }
        }
        pin[channel[0].T].curTrack.insert(TopTrack);
        HPos[channel[0].T][TopTrack]=0;
    }
    else {
        gdt<<"b{"<<channel[0].T<<" xy("<<0<<" "<<H<<" "<<0<<" "<<TrackNum<<" "<<0.1<<" "<<TrackNum<<" "<<0.1<<" "<<H<<") }\n";
        gdt<<"b{"<<channel[0].B<<" xy("<<0<<" "<<1<<" "<<0<<" "<<0<<" "<<0.1<<" "<<0<<" "<<0.1<<" "<<1<<") }\n";
        
        int TopTrack, BotTrack;
        for (auto t=trackAssign[0].begin(); t!=trackAssign[0].end(); t++) {
            if (t->second == channel[0].T)
                TopTrack = t->first;
            if (t->second == channel[0].B)
                BotTrack = t->first;
        }
        pin[channel[0].T].curTrack.insert(TopTrack);
        pin[channel[0].B].curTrack.insert(BotTrack);
        HPos[channel[0].T][TopTrack] = 0;
        HPos[channel[0].B][BotTrack] = 0;
    }

    int cha=1;
    auto A=trackAssign.begin(); A++;
    for (auto C=A;  C!=trackAssign.end(); C++, cha++) {
        // if (channel[cha].T != 0)
        //     VConnect.insert(channel[cha].T); 
        // if (channel[cha].B != 0)
        //     VConnect.insert(channel[cha].B); 
        Bi = channel[cha].B;
        Ti = channel[cha].T;
        for (auto t=C->begin(); t!=C->end(); t++) {
            int track = t->first;
            int P = t->second;
            if (t->second == -1) 
                continue;
            int p=t->second;
            int lastP = trackAssign[cha-1][t->first].second;
            if (trackAssign[cha-1][t->first].second != p && p!=-1) {
                HPos[p][t->first] = cha;
                pin[P].curTrack.insert(track);
            }
            if (cha<trackAssign.size()-1 && trackAssign[cha+1][t->first].second != p && p!=-1) {///////////////////have turn direction(verticle)
                gdt<<"b{"<<p<<" xy("<<HPos[p][t->first]<<" "<<t->first+1+0.1<<" "<<HPos[p][t->first]<<" "<<t->first+1<<" "
                   <<cha<<" "<<t->first+1<<" "<<cha<<" "<<t->first+1+0.1<<") }\n";
                std::cout<<"b{"<<p<<" xy("<<HPos[p][t->first]<<" "<<t->first+1+0.1<<" "<<HPos[p][t->first]<<" "<<t->first+1<<" "
                   <<cha<<" "<<t->first+1<<" "<<cha<<" "<<t->first+1+0.1<<") }\n";
                HPos[p].erase(t->first);
                VConnect.insert(p);
                // if (channel[cha].T == p) {
                //     gdt<<"b{"<<p<<" xy("<<cha<<" "<<T<<" "<<cha<<" "<<*pin[p].curTrack.rbegin()<<" "<<cha+0.1<<" "
                //        <<*pin[p].curTrack.rbegin()<<" "<<cha+0.1<<" "<<T<<") }\n";
                // }
            }
        }
        // printHPos(HPos);
        /*// for (auto p=VConnect.begin(); p!=VConnect.end(); p++) {
        //     int P(*p);
        //     if (channel[cha].T == *p && *p!=0) {
        //         int rt=*pin[*p].curTrack.rbegin();
        //         int t=*pin[*p].curTrack.begin();
        //         int test;
        //         if (cha<channelSize-1)
        //             test = trackAssign[cha+1][*pin[*p].curTrack.begin()].second;
        //         if (cha < channelSize-1 && *pin[*p].curTrack.rbegin() != *pin[*p].curTrack.begin() && trackAssign[cha+1][*pin[*p].curTrack.begin()].second != *p) {
        //             gdt<<"b{"<<*p<<" xy("<<cha<<" "<<H<<" "<<cha<<" "<<*pin[*p].curTrack.begin()+1<<" "<<cha+0.1<<" "
        //                 <<*pin[*p].curTrack.begin()+1<<" "<<cha+0.1<<" "<<H<<") }\n";
        //             std::cout<<"b{"<<*p<<" xy("<<cha<<" "<<H<<" "<<cha<<" "<<*pin[*p].curTrack.begin()+1<<" "<<cha+0.1<<" "
        //                 <<*pin[*p].curTrack.begin()+1<<" "<<cha+0.1<<" "<<H<<") }\n";
        //             int saveTrack(-1);
        //             for (auto delT=pin[*p].curTrack.begin(); delT!=pin[*p].curTrack.end(); delT++) {
        //                 if (trackAssign[cha+1][*delT].second == *p)
        //                     saveTrack = *delT;
        //             }
        //             pin[*p].curTrack.clear();
        //             if (saveTrack != -1)
        //                 pin[*p].curTrack.insert(saveTrack);
        //         }
        //         else {
        //             gdt<<"b{"<<*p<<" xy("<<cha<<" "<<H<<" "<<cha<<" "<<*pin[*p].curTrack.rbegin()+1<<" "<<cha+0.1<<" "
        //                 <<*pin[*p].curTrack.rbegin()+1<<" "<<cha+0.1<<" "<<H<<") }\n";
        //             std::cout<<"b{"<<*p<<" xy("<<cha<<" "<<H<<" "<<cha<<" "<<*pin[*p].curTrack.rbegin()+1<<" "<<cha+0.1<<" "
        //                 <<*pin[*p].curTrack.rbegin()+1<<" "<<cha+0.1<<" "<<H<<") }\n";
        //         }
                
        //     }
        //     if (channel[cha].B == *p && *p!=0) {
        //         int rt=*pin[*p].curTrack.rbegin();
        //         int t=*pin[*p].curTrack.begin();
        //         int test;
        //         if (cha<channelSize-1)
        //             test = trackAssign[cha+1][*pin[*p].curTrack.rbegin()].second;
        //         if (cha < channelSize-1 && *pin[*p].curTrack.rbegin() != *pin[*p].curTrack.begin() && trackAssign[cha+1][*pin[*p].curTrack.rbegin()].second != *p) {
        //             gdt<<"b{"<<*p<<" xy("<<cha<<" "<<*pin[*p].curTrack.rbegin()+1<<" "<<cha<<" "<<L<<" "<<cha+0.1<<" "
        //             <<L<<" "<<cha+0.1<<" "<<*pin[*p].curTrack.rbegin()+1<<") }\n";
        //         std::cout<<"b{"<<*p<<" xy("<<cha<<" "<<*pin[*p].curTrack.rbegin()+1<<" "<<cha<<" "<<L<<" "<<cha+0.1<<" "
        //             <<L<<" "<<cha+0.1<<" "<<*pin[*p].curTrack.rbegin()+1<<") }\n";
        //             int saveTrack(-1);
        //             for (auto delT=pin[*p].curTrack.begin(); delT!=pin[*p].curTrack.end(); delT++) {
        //                 if (trackAssign[cha+1][*delT].second == *p)
        //                 saveTrack = *delT;
        //             }
        //             pin[*p].curTrack.clear();
        //             if (saveTrack != -1)
        //                 pin[*p].curTrack.insert(saveTrack);
        //         }
        //         else {
        //             gdt<<"b{"<<*p<<" xy("<<cha<<" "<<*pin[*p].curTrack.begin()+1<<" "<<cha<<" "<<L<<" "<<cha+0.1<<" "
        //                 <<L<<" "<<cha+0.1<<" "<<*pin[*p].curTrack.begin()+1<<") }\n";
        //             std::cout<<"b{"<<*p<<" xy("<<cha<<" "<<*pin[*p].curTrack.begin()+1<<" "<<cha<<" "<<L<<" "<<cha+0.1<<" "
        //                 <<L<<" "<<cha+0.1<<" "<<*pin[*p].curTrack.begin()+1<<") }\n";
        //         }
                
        //     }
        //     if (*p!=channel[cha].B && *p!=channel[cha].T && *p!=0) {
        //         gdt<<"b{"<<*p<<" xy("<<cha<<" "<<*pin[*p].curTrack.rbegin()+1<<" "<<cha<<" "<<*pin[*p].curTrack.begin()+1<<" "
        //             <<cha+0.1<<" "<<*pin[*p].curTrack.begin()+1<<" "<<cha+0.1<<" "<<*pin[*p].curTrack.rbegin()+1<<") }\n";
        //         std::cout<<"b{"<<*p<<" xy("<<cha<<" "<<*pin[*p].curTrack.rbegin()+1<<" "<<cha<<" "<<*pin[*p].curTrack.begin()+1<<" "
        //             <<cha+0.1<<" "<<*pin[*p].curTrack.begin()+1<<" "<<cha+0.1<<" "<<*pin[*p].curTrack.rbegin()+1<<") }\n";
        //     }
        //     // for (auto t=pin[*p].curTrack.begin())*/
        }
        // VConnect.clear();
        
        
    for (int C=0; C<VerticleWire.size(); C++) {
        // for (int t=0; t<VerticleWire[C].size(); t++) {
        int Ti(channel[C].T), Bi(channel[C].B);
        int topT(0), botT(0);
        if (channel[C].B!=0) {
            topT=FindVerTrack(0,channel[C].B,C);
            gdt<<"b{"<<Bi<<" xy("<<C<<" "<<topT+1<<" "<<C<<" "<<0<<" "<<C+0.1<<" "
                <<0<<" "<<C+0.1<<" "<<topT+1<<") }\n";
            std::cout<<"b{"<<Bi<<" xy("<<C<<" "<<topT+1<<" "<<C<<" "<<0<<" "<<C+0.1<<" "
                <<0<<" "<<C+0.1<<" "<<topT+1<<") }\n";
        }
        else {
            topT=FindVerTrack(0,VerticleWire[C][0],C);
            int p=VerticleWire[C][0];
            if (VerticleWire[C][0] != -1) {
                gdt<<"b{"<<p<<" xy("<<C<<" "<<topT+1<<" "<<C<<" "<<botT+1<<" "<<C+0.1<<" "
                    <<botT+1<<" "<<C+0.1<<" "<<topT+1<<") }\n";
                std::cout<<"b{"<<p<<" xy("<<C<<" "<<topT+1<<" "<<C<<" "<<botT+1<<" "<<C+0.1<<" "
                    <<botT+1<<" "<<C+0.1<<" "<<topT+1<<") }\n";
            }
            // botT = ++topT;
        }
        botT = ++topT;
        while(topT<TrackNum) {
            int p=VerticleWire[C][botT];
            topT=FindVerTrack(botT, p, C);
            if (p==-1) {
                botT = ++topT;
                continue;
            }
            if (topT==TrackNum-1 && channel[C].T==VerticleWire[C][topT]) {
                gdt<<"b{"<<p<<" xy("<<C<<" "<<H<<" "<<C<<" "<<botT+1<<" "<<C+0.1<<" "
                    <<botT+1<<" "<<C+0.1<<" "<<H<<") }\n";
                std::cout<<"b{"<<p<<" xy("<<C<<" "<<H<<" "<<C<<" "<<botT+1<<" "<<C+0.1<<" "
                    <<botT+1<<" "<<C+0.1<<" "<<H<<") }\n";
            }
            else {
                gdt<<"b{"<<p<<" xy("<<C<<" "<<topT+1<<" "<<C<<" "<<botT+1<<" "<<C+0.1<<" "
                    <<botT+1<<" "<<C+0.1<<" "<<topT+1<<") }\n";
                std::cout<<"b{"<<p<<" xy("<<C<<" "<<topT+1<<" "<<C<<" "<<botT+1<<" "<<C+0.1<<" "
                    <<botT+1<<" "<<C+0.1<<" "<<topT+1<<") }\n";
            }
            botT = ++topT;
        }
    }
    gdt<<"}\n}\n";
    std::cout<<"}\n}\n";
    gdt.close();
}

void Router::printTrackAssign(){
    int idx(TrackNum-1);
    for (int i=TrackNum-1; i>=0; i--)
        std::cout<<"track: "<<trackAssign[c][i].first<<" "<<std::setw(5)<<trackAssign[c][i].second<<" VWire: "<<VWire[i]<<" Is empty? "<<emptyTrack[i]<< "\n";
    std::cout<<"\n";
}

void Router::printTrackAssign(int C){
    int idx(TrackNum-1);
    for (auto t=trackAssign[C].rbegin(); t!=trackAssign[C].rend(); t++) {
        std::cout<<"track: "<<t->first<<" "<<t->second<<" VWire: "<<VWire[idx]<< "\n";
        idx--;
    }std::cout<<"\n";
}

void Router::printChannelSize() {
    std::cout<<"channel ";
    for (int C=0; C<c; C++)
        std::cout<<std::setw(5)<<C<<" ";
    std::cout<<"\n        ";
    for (int C=0; C<c; C++) 
        std::cout<<std::setw(5)<<trackAssign[C].size()<<" ";
    std::cout<<"\n";
    

}

void Router::printChannelH() {
    std::cout<<" ";
    for (int i=0; i<trackAssign.size(); i++) {
        std::cout<<std::setw(5)<<i;
    }
    std::cout<<"\n";
    for (int i=TrackNum-1; i>=0; i--) {
        std::cout<<i;
        for (int j=0; j<trackAssign.size(); j++) {
            std::cout<<std::setw(5)<<trackAssign[j][i].second;
        }std::cout<<"\n";
    }
}

void Router::printChannelV() {
    std::cout<<" ";
    for (int i=0; i<VerticleWire.size(); i++) {
        std::cout<<std::setw(5)<<i;
    }
    std::cout<<"\n";
    for (int i=TrackNum-1; i>=0; i--) {
        std::cout<<i;
        for (int j=0; j<VerticleWire.size(); j++) {
            std::cout<<std::setw(5)<<VerticleWire[j][i];
        }std::cout<<"\n";
    }
}

void Router::printChannelV(int C) {
    std::cout<<" ";
    for (int i=0; i<VerticleWire.size(); i++) {
        std::cout<<std::setw(5)<<i;
    }
    std::cout<<"\n";
    for (int i=TrackNum-1; i>=0; i--) {
        std::cout<<i;
        for (int j=0; j<=C; j++) {
            std::cout<<std::setw(5)<<VerticleWire[j][i];
        }std::cout<<"\n";
    }
}

void Router::testPinTrack() {
    for (auto p=pin.begin(); p!=pin.end(); p++) {
        if (!p->second.curTrack.empty() && *p->second.curTrack.begin() == -1)
            std::cout<<"pin "<<p->first<<" have wrong track!\n";
    }
}

void Router::printHPos(std::map<int,std::map<int,int>>& HPos) {
    for (auto p=HPos.begin(); p!=HPos.end(); p++) {
        std::cout<<"pin "<<p->first<<" :\n";
        for (auto t=p->second.begin(); t!=p->second.end(); t++){
            std::cout<<"track: "<<t->first<<" pos(channel): "<<t->second<<"\n";
        }
    }std::cout<<"\n";
}
#endif