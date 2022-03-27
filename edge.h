#pragma once
#include<bits/stdc++.h>
#include"pos.h"

class Edge{
public:
    Pos start_pos;
    Pos end_pos;
    double len;
    double Len(){ return dist(start_pos,end_pos); }
    Edge( Pos s=Pos(),Pos e=Pos() ){ start_pos=s; end_pos=e; len=dist(start_pos,end_pos); }
    void operator=(Edge& a){
        start_pos=a.start_pos;
        end_pos=a.end_pos;
        len=a.len;
    }
    void operator=(const Edge& a){
        start_pos=a.start_pos;
        end_pos=a.end_pos;
        len=a.len;
    }
    bool operator==(const Edge &a){
        return (start_pos==a.start_pos && end_pos==a.end_pos);
    }
    bool operator!=(const Edge &a){
        return (start_pos!=a.start_pos || end_pos!=a.end_pos);
    }
    bool operator<(const Edge &a){
        return dist(start_pos,end_pos)<dist(a.start_pos,a.end_pos);
    }
    bool operator<=(const Edge &a){
        return dist(start_pos,end_pos)<=dist(a.start_pos,a.end_pos);
    }
    bool operator>(const Edge &a){
        return dist(start_pos,end_pos)>dist(a.start_pos,a.end_pos);
    }
    bool operator>=(const Edge &a){
        return dist(start_pos,end_pos)>=dist(a.start_pos,a.end_pos);
    }
};
