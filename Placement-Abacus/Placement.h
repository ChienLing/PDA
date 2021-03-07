#ifndef _PLACEMENT_H_
#define _PLACEMENT_H_
#define X first
#define Y second
#include <bits/stdc++.h>
#include "vertex.h"
#include "Row.h"
#include "Cluster.h"
class Placement {
    public:
    int NumRow;
    int NumNode;
    int NumTerminal;
    long long totalCost;
    std::vector<vertex> vertexVec;
    std::vector<std::map<int,int>> RowCut;
    std::map<std::string, int> nodeNameToIdx;
    std::vector<Row> InitRowVec;
    std::vector<std::vector<Row>> rowVec;
    Placement();
    void cutRow();
    void Abacus();
    long long PlaceRow(int , int, int, bool);
    void AddCell(Cluster*, int);
    void AddCluster(Cluster*, Cluster*);
    void Collapse (Cluster*);
    void clearCluster(Cluster* c);
    int BinarySearchY(int, int , int);
    int BinarySearchX(int Xpos, int r, int star, int end);
    void totalRow();
    void outputGDT(const std::string);
    void OutputFile(const std::string FileName);
    void printRowVec();
    void printVertexVec();
};

Placement::Placement():totalCost(0){}

void Placement::totalRow() {
    int TotalRow(0);
    for (int i=0; i<NumRow; i++) {
        TotalRow += rowVec[i].size();
    }
    std::cout<<"Total Row Number: "<<TotalRow<<"\n";
}

void Placement::Abacus() {
    sort(vertexVec.begin(), vertexVec.end());
    
    // printVertexVec();
    for (int n=0; n<vertexVec.size(); n++) {
        if (vertexVec[n].fixed)
            continue;

        int c_best(INT_MAX), rv_best(0), rh_best(0);
        bool changed1(false), changed2(false);
        int r = BinarySearchY(vertexVec[n].Coord.Y, 0, NumRow-1);
        int forwardv(r), backwardv(r-1);
        
        int mode1(0);
        long long cost(0);
        while (++mode1) {
            if (mode1&1 && forwardv<NumRow) {
                long long HdistSquare = (rowVec[forwardv].begin()->CoordY-vertexVec[n].Coord.Y)*(rowVec[forwardv].begin()->CoordY-vertexVec[n].Coord.Y);
                if (HdistSquare > c_best)
                    break;
                int r2 = BinarySearchX(vertexVec[n].Coord.X, forwardv, 0, rowVec[forwardv].size()-1);
                int forwardh(r2), backwardh(r2-1);
                int mode2(0);
                bool mode2_odd(true), mode2_even(true);
                while (++mode2) {
                    if (mode2&1 && forwardh<rowVec[forwardv].size() && mode2_odd) {
                        long long WdistSquare(0);
                        if (rowVec[forwardv][forwardh].CoordX >= vertexVec[n].Coord.X)
                            WdistSquare = (rowVec[forwardv][forwardh].CoordX - vertexVec[n].Coord.X)*(rowVec[forwardv][forwardh].CoordX - vertexVec[n].Coord.X);
                        else if (rowVec[forwardv][forwardh].Xmax-vertexVec[n].Width <= vertexVec[n].Coord.X)
                            WdistSquare = (rowVec[forwardv][forwardh].Xmax-vertexVec[n].Width - vertexVec[n].Coord.X)*(rowVec[forwardv][forwardh].Xmax-vertexVec[n].Width - vertexVec[n].Coord.X);
                        if ((WdistSquare+HdistSquare) > c_best) {
                            mode2_odd = false;
                            continue;
                        }
                        rowVec[forwardv][forwardh].nodeVec.push_back(n);
                        int cost = PlaceRow(n, forwardv, forwardh, false);
                        if (rowVec[forwardv][forwardh].nodeVec.back() != n)
                            std::cout<<"vertex n which in row wasn't last vertex!\n";
                        rowVec[forwardv][forwardh].nodeVec.pop_back();
                        if (cost < c_best) {
                            c_best = cost;
                            rv_best = forwardv;
                            rh_best = forwardh;
                            if (changed2) {
                                changed1 = true;
                                break;
                            }
                            changed2 = true;
                        }
                        forwardh++;
                    }
                    else if (!(mode2&1) && backwardh >=0 && mode2_even) {
                        long long WdistSquare(0);
                        if (rowVec[forwardv][backwardh].CoordX >= vertexVec[n].Coord.X)
                            WdistSquare = (rowVec[forwardv][backwardh].CoordX - vertexVec[n].Coord.X)*(rowVec[forwardv][backwardh].CoordX - vertexVec[n].Coord.X);
                        else if (rowVec[forwardv][backwardh].Xmax-vertexVec[n].Width <= vertexVec[n].Coord.X)
                            WdistSquare = (rowVec[forwardv][backwardh].Xmax-vertexVec[n].Width - vertexVec[n].Coord.X)*(rowVec[forwardv][backwardh].Xmax-vertexVec[n].Width - vertexVec[n].Coord.X);
                        if ((WdistSquare+HdistSquare) > c_best) {
                            mode2_even = false;
                            continue;
                        }
                        rowVec[forwardv][backwardh].nodeVec.push_back(n);
                        int cost = PlaceRow(n, forwardv, backwardh, false);
                        if (rowVec[forwardv][backwardh].nodeVec.back() != n)
                            std::cout<<"vertex n which in row wasn't last vertex!\n";
                        rowVec[forwardv][backwardh].nodeVec.pop_back();
                        if (cost < c_best) {
                            c_best = cost;
                            rv_best = forwardv;
                            rh_best = backwardh;
                            if (changed2) {
                                changed1 = true;
                                break;
                            }
                            changed2 = true;
                        }
                        backwardh--;
                    }
                    if (forwardh >= rowVec[forwardv].size())
                        mode2_odd = false;
                    if (backwardh <0)
                        mode2_even = false;
                    if (!mode2_odd && !mode2_even)
                        break;
                }
                forwardv++;
            }
            else if (!(mode1&1) && backwardv>=0 ) {
                long long HdistSquare = (rowVec[backwardv].begin()->CoordY-vertexVec[n].Coord.Y)*(rowVec[backwardv].begin()->CoordY-vertexVec[n].Coord.Y);
                if (HdistSquare > c_best)
                    break;
                int r2 = BinarySearchX(vertexVec[n].Coord.X, backwardv, 0, rowVec[backwardv].size()-1);
                int forwardh(r2), backwardh(r2-1);
                int mode2(0);
                bool mode2_odd(true), mode2_even(true);
                while (++mode2) {
                    if (mode2&1 && forwardh<rowVec[backwardv].size() && mode2_odd) {
                        long long WdistSquare(0);
                        if (rowVec[backwardv][forwardh].CoordX >= vertexVec[n].Coord.X)
                            WdistSquare = (rowVec[backwardv][forwardh].CoordX - vertexVec[n].Coord.X)*(rowVec[backwardv][forwardh].CoordX - vertexVec[n].Coord.X);
                        else if (rowVec[backwardv][forwardh].Xmax-vertexVec[n].Width <= vertexVec[n].Coord.X)
                            WdistSquare = (rowVec[backwardv][forwardh].Xmax-vertexVec[n].Width - vertexVec[n].Coord.X)*(rowVec[backwardv][forwardh].Xmax-vertexVec[n].Width - vertexVec[n].Coord.X);
                        if ((WdistSquare+HdistSquare) > c_best) {
                            mode2_odd = false;
                            continue;
                        }
                        rowVec[backwardv][forwardh].nodeVec.push_back(n);
                        int cost = PlaceRow(n, backwardv, forwardh, false);
                        if (rowVec[backwardv][forwardh].nodeVec.back() != n)
                            std::cout<<"vertex n which in row wasn't last vertex!\n";
                        rowVec[backwardv][forwardh].nodeVec.pop_back();
                        if (cost < c_best) {
                            c_best = cost;
                            rv_best = backwardv;
                            rh_best = forwardh;
                            if (changed2) {
                                changed1 = true;
                                break;
                            }
                            changed2 = true;
                        }
                        forwardh++;
                    }
                    else if (!(mode2&1) && backwardh >=0 && mode2_even) {
                        long long WdistSquare(0);
                        if (rowVec[backwardv][backwardh].CoordX >= vertexVec[n].Coord.X)
                            WdistSquare = (rowVec[backwardv][backwardh].CoordX - vertexVec[n].Coord.X)*(rowVec[backwardv][backwardh].CoordX - vertexVec[n].Coord.X);
                        else if (rowVec[backwardv][backwardh].Xmax-vertexVec[n].Width <= vertexVec[n].Coord.X)
                            WdistSquare = (rowVec[backwardv][backwardh].Xmax-vertexVec[n].Width - vertexVec[n].Coord.X)*(rowVec[backwardv][backwardh].Xmax-vertexVec[n].Width - vertexVec[n].Coord.X);
                        if ((WdistSquare+HdistSquare) > c_best) {
                            mode2_even = false;
                            continue;
                        }
                        rowVec[backwardv][backwardh].nodeVec.push_back(n);
                        int cost = PlaceRow(n, backwardv, backwardh, false);
                        if (rowVec[backwardv][backwardh].nodeVec.back() != n)
                            std::cout<<"vertex n which in row wasn't last vertex!\n";
                        rowVec[backwardv][backwardh].nodeVec.pop_back();
                        if (cost < c_best) {
                            c_best = cost;
                            rv_best = backwardv;
                            rh_best = backwardh;
                            if (changed2) {
                                changed1 = true;
                                break;
                            }
                            changed2 = true;
                        }
                        backwardh--;
                    }
                    if (forwardh>=rowVec[backwardv].size())
                        mode2_odd = false;
                    if (backwardh < 0)
                        mode2_even = false;
                    if (!mode2_odd && !mode2_even)
                        break;
                }
                backwardv--;
            }
        }
        rowVec[rv_best][rh_best].nodeVec.push_back(n);
        totalCost += PlaceRow(n, rv_best, rh_best, 1);
        // totalCost += c_best;
    }
    
}

long long Placement::PlaceRow(int n, int r, int subr, bool final) {
        if (rowVec[r][subr].totalCellWidth + vertexVec[n].Width > rowVec[r][subr].Width)
            return INT_MAX;
        for (auto v=rowVec[r][subr].nodeVec.begin(); v!=rowVec[r][subr].nodeVec.end(); v++) {
            auto c_last = rowVec[r][subr].cluster;//last cluster in row r
            while (c_last && c_last->next)
                c_last = c_last->next;
            if (v==rowVec[r][subr].nodeVec.begin() || (c_last && c_last->x_c + c_last->w_c <= vertexVec[*v].Coord.X) ) {
                Cluster *c = new Cluster;
                c->Xmin = rowVec[r][subr].CoordX;
                c->Xmax = rowVec[r][subr].Xmax;
                c->n_first = *v;
                // determine left side of cluster
                int b = vertexVec[*v].Coord.X;/////
                if (v==rowVec[r][subr].nodeVec.begin() && vertexVec[*v].Coord.X < rowVec[r][subr].CoordX)
                    c->x_c = rowVec[r][subr].CoordX;
                else if (v==rowVec[r][subr].nodeVec.begin() && vertexVec[*v].Coord.X > rowVec[r][subr].Xmax-vertexVec[*v].Width)
                    c->x_c = rowVec[r][subr].Xmax-vertexVec[*v].Width;
                else 
                    c->x_c = vertexVec[*v].Coord.X;
        
                AddCell(c,*v);
                if (c_last) {
                    c_last->next = c;
                    c->last = c_last;
                    c_last = c;
                }
                else {
                    rowVec[r][subr].cluster = c;
                }
                if (vertexVec[*v].Coord.X > rowVec[r][subr].Xmax-vertexVec[*v].Width)
                    Collapse(c);
            }
            else {
                AddCell(c_last,*v);
                Collapse(c_last);
            }
        }
    // Transform cluster positions c->x_c to cell positions x(i):
    Cluster* c = rowVec[r][subr].cluster;
    if (c && c!=c->last)
        std::cout<<"In vertex "<<n<<", rowVec["<<r<<"].cluster was not first pointer!!\n";
    if (final) {
        rowVec[r][subr].totalCellWidth += vertexVec[n].Width;
        while (c) {
            int x = c->x_c;
            for (auto v=c->cellVec.begin(); v!=c->cellVec.end(); v++) {
                int vertex = *v;
                vertexVec[*v].PlacedCoord.X = x;//here has difference with paper
                vertexVec[*v].PlacedCoord.Y = rowVec[r][subr].CoordY;
                x += vertexVec[*v].Width;
            }
            int a = rowVec[r][subr].CoordX;
            int b = rowVec[r][subr].Width;
            if (x > rowVec[r][subr].CoordX+rowVec[r][subr].Width) {
                clearCluster(rowVec[r][subr].cluster);/////////////////////////
                rowVec[r][subr].cluster = NULL;
                return INT_MAX;
            }
            c = c->next;
        }
        long long Hcost = (vertexVec[n].PlacedCoord.X-vertexVec[n].Coord.X)*(vertexVec[n].PlacedCoord.X-vertexVec[n].Coord.X);
        long long Vcost = (vertexVec[n].PlacedCoord.Y-vertexVec[n].Coord.Y)*(vertexVec[n].PlacedCoord.Y-vertexVec[n].Coord.Y);
        long long cost = Hcost+Vcost;
        c = rowVec[r][subr].cluster;
        return cost;
    }
    else {
        while (c) {
            int x = c->x_c;
            for (auto v=c->cellVec.begin(); v!=c->cellVec.end(); v++) {
                int vertex = *v;
                vertexVec[*v].TmpCoord.X = x;//here has difference with paper
                vertexVec[*v].TmpCoord.Y = rowVec[r][subr].CoordY;
                x += vertexVec[*v].Width;
            }
            int a = rowVec[r][subr].CoordX;
            int b = rowVec[r][subr].Width;
            if (x > rowVec[r][subr].CoordX+rowVec[r][subr].Width) {
                clearCluster(rowVec[r][subr].cluster);/////////////////////////
                rowVec[r][subr].cluster = NULL;
                return INT_MAX;
            }
            c = c->next;
        }
        long long Hcost = (vertexVec[n].TmpCoord.X-vertexVec[n].Coord.X)*(vertexVec[n].TmpCoord.X-vertexVec[n].Coord.X);
        long long Vcost = (vertexVec[n].TmpCoord.Y-vertexVec[n].Coord.Y)*(vertexVec[n].TmpCoord.Y-vertexVec[n].Coord.Y);
        long long cost = Hcost+Vcost;
        clearCluster(rowVec[r][subr].cluster);/////////////////////////
        rowVec[r][subr].cluster = NULL;
        return cost;
    }
    
}

void Placement::AddCell(Cluster* c, int n) {
    c->cellVec.push_back(n);
    c->n_last = n;
    c->e_c += 1;  //c->e_c += vertexVec[n].weight; vertexVec[n].weight == 1
    c->q_c += vertexVec[n].Coord.X-c->w_c;
    c->w_c += vertexVec[n].Width;
    vertexVec[n].cluster = c;
}

void Placement::AddCluster(Cluster* c, Cluster* c0) {
    c->n_last = c0->n_last;
    c->cellVec.insert(c->cellVec.end(), c0->cellVec.begin(), c0->cellVec.end());
    c->e_c += c0->e_c;
    c->q_c += c0->q_c-(c0->e_c*c->w_c);
    c->w_c += c0->w_c;
    c->next = c0->next;
    if (c0->next)
        c0->next->last = c;
}

void Placement::Collapse(Cluster* c) {
    c->x_c = c->q_c/c->e_c;
    if (c->x_c < c->Xmin) 
        c->x_c = c->Xmin;
    if (c->x_c > c->Xmax-c->w_c)
        c->x_c = c->Xmax-c->w_c;
    //Overlap between c and its predecessor c'
    Cluster* c_last = c->last;
    if (c_last != c && c_last->x_c+c_last->w_c > c->x_c) {//Merge cluster c to c_last
        AddCluster(c_last, c);
        delete c;
        c=c_last;
        Collapse (c);
    }
    
}

int Placement::BinarySearchY(int H, int start, int end) {
    if (start==end)
        return start;//??
    int mid = (end+start)/2;
    if (InitRowVec[mid].CoordY > H)
        return BinarySearchY(H, start, mid);
    else if (InitRowVec[mid].CoordY < H)
        return BinarySearchY(H, mid+1, end);
    else
        return mid;
}

int Placement::BinarySearchX(int Xpos, int r, int start, int end) {
    if (start == end)
        return start;
    int mid = (start+end)/2;
    if (rowVec[r][mid].CoordX <= Xpos && rowVec[r][mid].CoordX+rowVec[r][mid].Width >= Xpos)//row left side < cell pos < row rignt side- cell Width
        return mid;
    else if (rowVec[r][mid].CoordX+rowVec[r][mid].Width < Xpos)//row right side < cell pos
        return BinarySearchX(Xpos, r, mid+1, end);
    else 
        return BinarySearchX(Xpos, r, start, mid);
}

void Placement::cutRow() {
    for (int i=vertexVec.size()-NumTerminal; i<vertexVec.size(); i++) {
        int a = i-vertexVec.size()+NumTerminal;
        if (!vertexVec[i].fixed)
            std::cout<<"cut row has wrong\n";
        // if (a==28)
        //     std::cout<<"stop\n";
        int rv= BinarySearchY(vertexVec[i].Coord.Y, 0, InitRowVec.size()-1);
        if (InitRowVec[rv].CoordY != vertexVec[i].Coord.Y)
            continue;
            // std::cout<<" cutRow function search wrong!\n";
        while (InitRowVec[rv].CoordY < vertexVec[i].Coord.Y+vertexVec[i].Height && rv<NumRow) {
            RowCut[rv][vertexVec[i].Coord.X]++;
            RowCut[rv][vertexVec[i].Coord.X+vertexVec[i].Width]--;
            rv++;
        }
    }

    for (auto i=0; i<NumRow; i++) {
        if (RowCut[i].size() == 0) {
            rowVec[i].push_back(InitRowVec[i]);
            continue;
        }
        int total(0);
        int rightBoundary = InitRowVec[i].Xmax;
        auto ptr = RowCut[i].begin();
        bool insert(true);
        int leftSide(InitRowVec[i].CoordX), rightSide(0);
        while (ptr != RowCut[i].end()) {
            total += ptr->second;
            if (total != 0 && insert) {
                rightSide = ptr->first;
                insert = false;
                if (rightSide > rightBoundary) {
                    rightSide = rightBoundary;
                    Row R = InitRowVec[i];
                    R.CoordX = leftSide;
                    R.Xmax = rightSide;
                    R.Width = rightSide - leftSide;
                    rowVec[i].push_back(R);
                    break;
                }
                if (rightSide <= leftSide) {
                    continue;
                }
                else {
                    Row R = InitRowVec[i];
                    R.CoordX = leftSide;
                    R.Xmax = rightSide;
                    R.Width = rightSide - leftSide;
                    rowVec[i].push_back(R);
                }
            }
            if (total == 0 && !insert) {
                leftSide = ptr->first;
                insert = true;
            }
            ptr++;
        }
        if (total == 0 && leftSide < rightBoundary) {
            Row R = InitRowVec[i];
            R.CoordX = leftSide;
            R.Xmax = rightBoundary;
            R.Width = rightBoundary - leftSide;
            rowVec[i].push_back(R);
        }
        // std::cout<<" number of subrow in row vector "<<i<<": "<<rowVec[i].size()<<"\n";
    }
}


void Placement::OutputFile(const std::string FileName) {
    std::ofstream output;
    output.open(FileName);
    if (!output) {
        std::cerr<<"Can not open "<<FileName<<"!\n";
        exit(1);
    }
    for (int i=0; i<vertexVec.size(); i++) {
        if (!vertexVec[i].fixed) {
            output<<vertexVec[i].vertexName<<" "<<vertexVec[i].PlacedCoord.X<<" "<<vertexVec[i].PlacedCoord.Y<<" : N\n";
        }
        else {
            output<<vertexVec[i].vertexName<<" "<<vertexVec[i].Coord.X<<" "<<vertexVec[i].Coord.Y<<" : N FIXED\n";
        }
    }
    output.close();
} 

void Placement::clearCluster(Cluster* c) {
    while (c) {
        Cluster* c0 = c->next;
        delete c;
        c = c0;
    }
}

void Placement::outputGDT(const std::string gdtFile) {
    std::ofstream gdt(gdtFile);
    if (!gdt) {
        std::cerr<<"Can not open"<<gdtFile<<"!\n";
        exit(1);
    }
    gdt<<"gds2{600\n"
       <<"m=2018-09-14 14:26:15 a=2018-09-14 14:26:15\n"
       <<"lib 'asap7sc7p5t_24_SL' 0.00025 2.5e-10\n"
       <<"cell{c=2018-09-14 14:26:15 m=2018-09-14 14:26:15 'AND2x2_ASAP7_75t_SL'\n";
        for (int i=0; i<rowVec.size(); i++) {
            for (int j=0; j<rowVec[i].size(); j++) {
                gdt<<"b{1 xy("<<rowVec[i][j].CoordX<<" "<<rowVec[i][j].CoordY+rowVec[i][j].Height<<" "
                          <<rowVec[i][j].CoordX<<" "<<rowVec[i][j].CoordY<<" "
                          <<rowVec[i][j].CoordX+rowVec[i][j].Width<<" "<<rowVec[i][j].CoordY<<" "
                          <<rowVec[i][j].CoordX+rowVec[i][j].Width<<" "<<rowVec[i][j].CoordY+rowVec[i][j].Height<<") }\n";
            }
            
        }
        for (int i=0; i<vertexVec.size(); i++) {
            if (vertexVec[i].fixed) {
                gdt<<"b{2 xy("<<vertexVec[i].Coord.X<<" "<<vertexVec[i].Coord.Y+vertexVec[i].Height<<" "
                              <<vertexVec[i].Coord.X<<" "<<vertexVec[i].Coord.Y<<" "
                              <<vertexVec[i].Coord.X+vertexVec[i].Width<<" "<<vertexVec[i].Coord.Y<<" "
                              <<vertexVec[i].Coord.X+vertexVec[i].Width<<" "<<vertexVec[i].Coord.Y+vertexVec[i].Height<<") }\n";
            }
            else {
                gdt<<"b{3 xy("<<vertexVec[i].PlacedCoord.X<<" "<<vertexVec[i].PlacedCoord.Y+vertexVec[i].Height<<" "
                              <<vertexVec[i].PlacedCoord.X<<" "<<vertexVec[i].PlacedCoord.Y<<" "
                              <<vertexVec[i].PlacedCoord.X+vertexVec[i].Width<<" "<<vertexVec[i].PlacedCoord.Y<<" "
                              <<vertexVec[i].PlacedCoord.X+vertexVec[i].Width<<" "<<vertexVec[i].PlacedCoord.Y+vertexVec[i].Height<<") }\n";
                gdt<<"b{4 xy("<<vertexVec[i].Coord.X<<" "<<vertexVec[i].Coord.Y+vertexVec[i].Height<<" "
                              <<vertexVec[i].Coord.X<<" "<<vertexVec[i].Coord.Y<<" "
                              <<vertexVec[i].Coord.X+vertexVec[i].Width<<" "<<vertexVec[i].Coord.Y<<" "
                              <<vertexVec[i].Coord.X+vertexVec[i].Width<<" "<<vertexVec[i].Coord.Y+vertexVec[i].Height<<") }\n";
            }
        }
        gdt<<"}\n}\n";
        gdt.close();
}

void Placement::printVertexVec() {
    for (int i=1228177; i<vertexVec.size(); i++)
        vertexVec[i].printVertex();
}

void Placement::printRowVec() {
    std::cout<<"Row Vector Size: "<<rowVec.size()<<"\n";
    for (int i=0; i<rowVec.size(); i++) {
        for (int j=0; j<rowVec[i].size(); j++) 
            rowVec[i][j].printRow();
    }
}
#endif