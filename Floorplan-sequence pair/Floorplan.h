#ifndef _FLOORPLAN_H_
#define _FLOORPLAN_H_
#include <bits/stdc++.h>
#define lowbit(x) (x&(-x))
class Floorplan {
    public:
    int outlineX, outlineY;
    int totalBlockX, totalBlockY;
    int numNet;
    int NumBlock;
    int NumTerminal;
    long long T;
    long double alpha;
    long long W;
    double  cost;
    long long resultArea;
    std::vector<int> Gamma1;
    std::vector<int> Gamma2;
    std::vector<std::vector<int>> netList;
    std::vector<std::pair<int,int>> blockWH;
    std::vector<std::pair<int,int>> blockXY;
    std::vector<std::pair<int,int>> bestWH;
    std::map<std::string, int> blockNum;
    std::map<int, std::string> blockName; 
    std::vector<std::pair<int,int>> terminalXY;
    std::map<std::string, int> terminalNum;
    std::map<int, std::string> terminalName; 
    
    Floorplan();

    void SA();
    void rotate(int, int, int, int, int);
    void change(int, int, int, int, int);
    void shift(int, int, int, int, int);
    void refineShift(int, int, int, int, int);
    void swap(int& a, int& b);
    std::pair<int,int> WLCS(std::vector<int>& , std::vector<int>& ,std::vector<std::pair<int,int>>&);
    void reorder(std::vector<int>& , std::vector<int>& );
    void reset();
    void getCost();
    void getW();
    long long getArea(int weight, int height);
    void returnVec(std::vector<int>& , std::vector<int>& );
    void printNet();
    void printBlock();
    void printVector(std::vector<int> &);
    void printTerminal();
    void reverse(std::vector<int> &);
    void printCoordinate();
    void printCoordinate (std::vector<std::pair<int,int>>& );
    void place();
    void outputGDT(const std::string);
};
Floorplan::Floorplan():outlineX(0), outlineY(0), W(0), totalBlockX(0), totalBlockY(0), cost(0){}

void Floorplan::swap(int& a, int& b) {
    int tmp=a; a=b; b=tmp;
}

void Floorplan::SA() {
    std::srand ( unsigned (std::time(0)));
    std::pair<int,int> tmpBlockLW;
    std::vector<std::pair<int,int>> blockCoor;
    blockXY.resize(NumBlock);
    double ratio(0);   long double P(0),p(0);   double oldCost(1e8);  double cost(0);
    int op;     int area(1e8);
    int times = 20000;
    while (times--) {
        if (times%1000==0)
            std::cout<<times<<"\n";
            // std::cout<<"time:"<<times<<"\n";
        op = rand()%100;

        int start1(rand()%NumBlock), end1(rand()%NumBlock);
        int start2(rand()%NumBlock), end2(rand()%NumBlock);
        int mode(rand()%3);
        while (start1==end1)
            // end1 = (end1+1)%NumBlock;
            end1 = rand()%NumBlock;
        while (start2==end2)
            // end2 = (end2+1)%NumBlock;
            end2 = rand()%NumBlock;
        if (start1 > end1)
            swap(start1, end1);
        if (start2 > end2)
            swap(start2, end2);
        if (start1 > end1 || start2 > end2)
            std::cout<<"shift has wrong!\n";

        // std::cout<<"Gamma1: ";printVector(Gamma1);
        // std::cout<<"Gamma2: ";printVector(Gamma2);
        if (op < 20 ) {  //rotate
            rotate(start1, start2, end1, end2, mode);
            // printBlock();
        }
        else if (op < 60) //swap one pair in Gamma1
            change(start1, start2, end1, end2, mode);
        else 
            shift(start1, start2, end1, end2, mode);
        
        // std::cout<<"Gamma1: ";printVector(Gamma1);
        // std::cout<<"Gamma2: ";printVector(Gamma2);
        tmpBlockLW = WLCS(Gamma1, Gamma2, blockCoor);
        // printCoordinate(blockCoor);
        area = tmpBlockLW.first*tmpBlockLW.second;
        double b=(double)tmpBlockLW.first/tmpBlockLW.second;
        double c=(double)outlineX/outlineY;
        ratio = b/c;
        if (ratio < 1)
            ratio = 1/ratio;
        if (tmpBlockLW.first > outlineX) 
            ratio *= 1.20;
        if (tmpBlockLW.second > outlineY)
            ratio *= 1.20;
        if (tmpBlockLW.first<=outlineX && tmpBlockLW.second <= outlineY) {
            bestWH = blockWH;
            totalBlockX = tmpBlockLW.first;
            totalBlockY = tmpBlockLW.second;
            blockXY = blockCoor;
            break;
        }
        
        cost = (double)area*ratio;
        P = exp(-1*(double)cost/T);     p=(double)rand()/RAND_MAX;
        if (cost<oldCost || p<P) {
            if (cost<oldCost) {
                bestWH = blockWH;
                totalBlockX = tmpBlockLW.first;
                totalBlockY = tmpBlockLW.second;
                blockXY = blockCoor;
                // std::cout<<"Convergence, area: "<<area<<" X: "<<tmpBlockLW.first<<" Y: "<<tmpBlockLW.second<<"\n";
            }
            // else if (p<P)
            //     std::cout<<"area raise: "<<area<<" X: "<<tmpBlockLW.first<<" Y: "<<tmpBlockLW.second<<"  Jump\n";
            oldCost = cost;
        }
        else {
            if (op < 20 )  //rotate
                rotate(start1, start2, end1, end2, mode);
            else if (op < 60) //swap one pair in Gamma1
                change(start1, start2, end1, end2, mode);
            else 
                refineShift(start1, start2, end1, end2, mode);
            // std::cout<<"2.return change Gamma2";printVector(Gamma2);
        }
        T *= 0.86;
    }
} 

void Floorplan::rotate(int start1, int start2, int end1, int end2, int mode) {
    
    if (mode == 0) {
        swap(blockWH[start1].first, blockWH[start1].second);
        // printBlock();
    }
    else if (mode == 1) {
        swap(blockWH[end1].first, blockWH[end1].second);
        // printBlock();
    }
    else if (mode == 2) {
        swap(blockWH[start1].first, blockWH[start1].second);
        swap(blockWH[end1].first, blockWH[end1].second);
        // printBlock();
    }
        
    
}

void Floorplan::change(int start1, int start2, int end1, int end2, int mode) {
    if (mode == 0)
        swap(Gamma1[start1], Gamma1[end1]);
    else if (mode == 1)
        swap(Gamma2[start2], Gamma2[end2]);
    else if (mode == 2) {
        swap(Gamma1[start1], Gamma1[end1]);
        swap(Gamma2[start2], Gamma2[end2]);
    }
    
}

void Floorplan::shift (int start1, int start2, int end1, int end2, int mode) {
    if (mode == 0) {
        int tmp = Gamma1[start1];
        for (int i=start1; i<end1; i++) 
            Gamma1[i] = Gamma1[i+1];
        Gamma1[end1] = tmp;
    }
    else if (mode == 1) {
        int tmp = Gamma2[start2];
        for (int i=start2; i<end2; i++) {
            Gamma2[i] = Gamma2[i+1];
        }
        Gamma2[end2] = tmp;
    }
    else if (mode == 2) {
        int tmp1 = Gamma1[start1];
        for (int i=start1; i<end1; i++) 
            Gamma1[i] = Gamma1[i+1];
        Gamma1[end1] = tmp1;
        
        int tmp2 = Gamma2[start2];
        for (int i=start2; i<end2; i++) {
            Gamma2[i] = Gamma2[i+1];
        }
        Gamma2[end2] = tmp2;
    }

}

void Floorplan::refineShift(int start1, int start2, int end1, int end2, int mode) {
    if (mode == 0) {
        int tmp1 = Gamma1[end1];
        for (int i=end1; i>start1; i--) 
            Gamma1[i] = Gamma1[i-1];
        Gamma1[start1] = tmp1;
    }
    else if (mode == 1) {
        int tmp2 = Gamma2[end2];
        for (int i=end2; i>start2; i--) {
            Gamma2[i] = Gamma2[i-1];
        }
        Gamma2[start2] = tmp2;
    }
    else if (mode == 2) {
        int tmp1 = Gamma1[end1];
        for (int i=end1; i>start1; i--) 
            Gamma1[i] = Gamma1[i-1];
        Gamma1[start1] = tmp1;
        
        int tmp2 = Gamma2[end2];
        for (int i=end2; i>start2; i--) {
            Gamma2[i] = Gamma2[i-1];
        }
        Gamma2[start2] = tmp2;
    }
}

long long Floorplan::getArea(int weight, int height) {
    return 1LL*weight*height;
}

std::pair<int,int> Floorplan::WLCS(std::vector<int>& vec1, std::vector<int>& vec2, std::vector<std::pair<int,int>>& blockCoor) {
    std::vector<int> Orivec1 = vec1;
    std::vector<int> Orivec2 = vec2;
    // printVector(vec1);    printVector(vec2);
    reorder(vec1, vec2);
    // printVector(vec1);    printVector(vec2);
    blockCoor.clear();
    blockCoor.resize(vec1.size(),{0,0});
    std::vector<int> DPX(vec2.size(),0);
    std::vector<int> DPY(vec2.size(),0);
    int maxX(0), maxY(0);
    for (int i=0; i<vec2.size(); i++) {
        int block=vec1[vec2[i]];
        for (int j=0; j<i; j++) {
            int lastBlock(vec1[vec2[j]]);
            if (vec2[j] < vec2[i]) {
                if (DPX[i] < DPX[j]+blockWH[lastBlock].first) {
                    DPX[i] = DPX[j]+blockWH[lastBlock].first;
                    blockCoor[block].first = DPX[i];
                }
            }
            // printVector(DPX);
        }
        // DPX[i] += blockWH[block].first;
        if (DPX[i]+blockWH[block].first > maxX) {
            maxX = DPX[i]+blockWH[block].first;
        }
    }
    returnVec(vec1,vec2);
    reverse(vec1);     
    reorder(vec1,vec2);
    // printVector(vec1);    printVector(vec2);
    for (int i=0; i<vec2.size(); i++) {
        int block=vec1[vec2[i]];
        for (int j=0; j<i; j++) {
            int lastBlock(vec1[vec2[j]]);
            if (vec2[j] < vec2[i]) {
                if (DPY[i] < DPY[j]+blockWH[lastBlock].second) {
                    DPY[i] = DPY[j]+blockWH[lastBlock].second;
                    blockCoor[block].second = DPY[i];
                }
            }
            // printVector(DPY);
        }
        // DPY[i] += blockWH[block].second;
        int a(blockWH[block].second);
        int b(DPY[i]);
        if (DPY[i]+blockWH[block].second > maxY) {
            maxY = DPY[i]+blockWH[block].second;
        }
    }
    
    returnVec(vec1, vec2);
    reverse(vec1);
    if (Orivec1 != vec1 || Orivec2 != vec2)
        std::cout<<"wrong\n";
    return {maxX, maxY};
}

void Floorplan::reorder(std::vector<int>& vec1, std::vector<int>& vec2) {
    std::vector<int> order(vec1.size());
    for (int i=0; i<vec1.size(); i++) 
        order[vec1[i]] = i;
    
    for (int i=0; i<vec1.size(); i++) 
        vec2[i] = order[vec2[i]];
    
}

void Floorplan::reverse(std::vector<int>& vec) {
    std::vector<int> vecTmp = vec;
    for (int i=0; i<vec.size(); i++) {
        vec[i] = vecTmp[vec.size()-i-1];
    }
}

void Floorplan::getCost () {
    long long totalArea(0);
    totalArea = totalBlockX*totalBlockY;
    cost = (double)alpha*totalArea + (double)(1-alpha)*W;
    // return cost;
}

void Floorplan::getW () {
    for (int i=0; i<netList.size(); i++) {
        int minX(1e8), minY(1e8), maxX(0), maxY(0);
        for (int j=0; j<netList[i].size(); j++) {
            // std::cout<<""
            if (netList[i][j] < NumBlock) {
                // std::cout<<"block : "<<blockName[netList[i][j]]<<" coordinate: ("<<blockXY[netList[i][j]].first<<","<<blockXY[netList[i][j]].second
                //          <<"), HW: ("<<blockWH[netList[i][j]].first<<","<<blockWH[netList[i][j]].second<<")\n";
                int midX = blockXY[netList[i][j]].first + blockWH[netList[i][j]].first/2;
                int midY = blockXY[netList[i][j]].second + blockWH[netList[i][j]].second/2;
                minX = std::min(midX, minX);
                minY = std::min(midY, minY);
                maxX = std::max(midX, maxX);
                maxY = std::max(midY, maxY);
            }
            else {
                // std::cout<<"terminal : "<<terminalName[netList[i][j]-NumBlock]<<" coordinate: ("<<terminalXY[netList[i][j]-NumBlock].first<<","<<terminalXY[netList[i][j]-NumBlock].second<<")\n";
                minX = std::min(terminalXY[netList[i][j]-NumBlock].first, minX);
                minY = std::min(terminalXY[netList[i][j]-NumBlock].second, minY);
                maxX = std::max(terminalXY[netList[i][j]-NumBlock].first, maxX);
                maxY = std::max(terminalXY[netList[i][j]-NumBlock].second, maxY);
            }
        }
        int HPWL = (maxX-minX) + (maxY-minY);
        W += HPWL;
    }
}

void Floorplan::printNet() {
    std::cout<<"Print Net:\n";
    for (int i=0; i<netList.size(); i++) {
        std::cout<<"Net "<<i<<": ";
        for (int j=0; j<netList[i].size(); j++) {
            if (netList[i][j] < NumBlock)
                std::cout<<blockName[netList[i][j]]<<" ";
            else 
                std::cout<<terminalName[netList[i][j]]<<" ";    
        }
        std::cout<<"\n";
    }
}

void Floorplan::printBlock() {
    std::cout<<"Print Blcok:\n";
    for (int i=0; i<blockWH.size(); i++) {
        std::cout<<blockName[i]<<": "<<blockWH[i].first<<" "<<blockWH[i].second<<"\n";
    }
}

void Floorplan::printVector(std::vector<int> & vec) {
    for (int i=0; i<vec.size(); i++) {
        std::cout<<vec[i]<<" ";
    }
    std::cout<<"\n\n";
}

void Floorplan::printTerminal() {
    std::cout<<"Print Terminal:\n";
    for (int i=0; i<terminalXY.size(); i++) {
        std::cout<<terminalName[i+NumBlock]<<": "<<terminalXY[i].first<<" "<<terminalXY[i].second<<"\n";
    }

}

void Floorplan::printCoordinate () {
    for (int i=0; i<blockXY.size(); i++) {
        std::cout<<"block: "<<i<<"("<<blockXY[i].first<<","<<blockXY[i].second<<")\n";
    }
}

void Floorplan::place() {
    int coorX(0), coorY(0);
    totalBlockX = 0;
    totalBlockY = 0;
    for (int i=0; i<NumBlock; i++) {
        blockXY[i].first=coorX;
        blockXY[i].second=coorY;
        coorX += blockWH[i].first;
        totalBlockX = coorX;
        totalBlockY = std::max(blockWH[i].second, totalBlockY);
    }
}

void Floorplan::printCoordinate (std::vector<std::pair<int,int>>& vec) {
    for (int i=0; i<vec.size(); i++) {
        std::cout<<"block: "<<i<<"("<<vec[i].first<<","<<vec[i].second<<")\n";
    }
}

void Floorplan::returnVec(std::vector<int>& vec1, std::vector<int>& vec2) {
    for (int i=0; i<vec2.size(); i++) {
        vec2[i] = vec1[vec2[i]];
    }
} 

void Floorplan::outputGDT(const std::string outputFile) {
    std::ofstream gdt(outputFile);
    if (!gdt) {
        std::cerr<<"Can not open "<<outputFile<<" !\n";
        exit(1);
    }
    gdt<<"gds2{600\n"
       <<"m=2018-09-14 14:26:15 a=2018-09-14 14:26:15\n"
       <<"lib 'asap7sc7p5t_24_SL' 0.00025 2.5e-10\n"
       <<"cell{c=2018-09-14 14:26:15 m=2018-09-14 14:26:15 'AND2x2_ASAP7_75t_SL'\n";
    // printBlock();
    // printCoordinate();
    for (auto b=0; b<blockXY.size(); b++) {
        gdt<<"b{"<<b<<" xy("<<blockXY[b].first<<" "<<blockXY[b].second+blockWH[b].second<<" "
           <<blockXY[b].first<<" "<<blockXY[b].second<<" "
           <<blockXY[b].first+blockWH[b].first<<" "<<blockXY[b].second<<" "
           <<blockXY[b].first+blockWH[b].first<<" "<<blockXY[b].second+blockWH[b].second<<") }\n";
        // std::cout<<"b{"<<b<<" xy("<<blockXY[b].first<<" "<<blockXY[b].second+blockWH[b].second<<" "
        //    <<blockXY[b].first<<" "<<blockXY[b].second<<" "
        //    <<blockXY[b].first+blockWH[b].first<<" "<<blockXY[b].second<<" "
        //    <<blockXY[b].first+blockWH[b].first<<" "<<blockXY[b].second+blockWH[b].second<<") }\n";
    }
    gdt<<"}\n}\n";
    gdt.close();
}
#endif