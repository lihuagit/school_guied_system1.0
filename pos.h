#ifndef POS_H
#define POS_H
#include<bits/stdc++.h>
struct Pos
{
    /* data */
    int x;
    int y;
    Pos(int xx=0,int yy=0) {
        x=xx;
        y=yy;
    }
    void operator=(const Pos &a){
        x=a.x;
        y=a.y;
    }
    bool operator!=(const Pos &a){
        return (x!=a.x && y!=a.y);
    }
    bool operator==(const Pos &a){
        if(std::abs(x-a.x)<=15 && std::abs(y-a.y)<=15)
            return true;
        return false;
    }
    bool operator==(Pos &a){
        if(std::abs(x-a.x)<=15 && std::abs(y-a.y)<=15)
            return true;
        return false;
    }
};

// bool operator==(Pos &a,Pos &b){
//     if(std::abs(b.x-a.x)<=15 && std::abs(b.y-a.y)<=15)
//         return true;
//     return false;
// }
double dist(Pos s,Pos e);

#endif