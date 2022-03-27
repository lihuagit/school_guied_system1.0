#pragma once
#include<bits/stdc++.h>
#include <QObject>
#include <QDebug>
#include"edge.h"

struct pos_cmp{
	bool operator()(Pos a,Pos b){
		if(abs(a.x-b.x)<=15 && abs(a.y-b.y)<=15)
			return false;
		return a.x+a.y<b.x+b.y;
	}
};
struct edge_com{
    bool operator()(Edge a,Edge b){
        if(abs(a.start_pos.x-b.start_pos.x)<=15 && abs(a.start_pos.y-b.start_pos.y)<=15)
            if(abs(a.end_pos.x-b.end_pos.x)<=15 && abs(a.end_pos.y-b.end_pos.y)<=15)
                return false;
        return dist(a.start_pos,a.end_pos)<=dist(b.start_pos,b.end_pos);
    }
};

class Map : public QObject{
    Q_OBJECT
private:
public:
    Map();
    bool AddEdge(Pos &Start,Pos &End);
    bool AddEPos(int x,int y);
    bool AddPlace(std::string name,Pos pos);
    bool ErasePlace(std::string name);
    void readpos();
    void readedge();
    void readplace();
    std::vector<Pos> dijkstra(std::string sname,std::string ename);
    std::vector<Pos> best_far(std::string sname,std::string ename);
    std::vector<Pos> ans_far_temp;
    double ans_far_dist_temp;
    std::vector<Pos> ans_far;
    double ans_far_dist;
    int ans_i;
    std::map<Pos,bool,pos_cmp> ans_far_vis;
    void best_far(Pos& s,Pos& e);
    std::set<Pos,pos_cmp> m_pos;
    std::set<Edge,edge_com> m_edge;
    std::map<std::string,Edge> m_place;
    std::map<Pos,std::vector<Edge>,pos_cmp> m_pos_edge;
    int num;
    double ans_len=0;
    double ans_k=0;
};