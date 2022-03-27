#include"map.h"

Map::Map(){
    readpos();
    readedge();
    readplace();
}

void Map::readpos(){
    std::ifstream fin;
    fin.open("./src/edge_pos.txt",std::ios::in);
    if(fin.good()){
        int x,y;
        while(!fin.eof()){
            fin>>x>>y;
            m_pos.insert(Pos(x,y));
        }
    }
    // else qDebug()<<"read error!!!";
    fin.close();
}
void Map::readedge(){
    std::ifstream fin;
    fin.open("./src/edge.txt",std::ios::in);
    if(fin.good()){
        int x1,y1;
        int x2,y2;
        while(!fin.eof()){
            fin>>x1>>y1;
            fin>>x2>>y2;
            if(x1+y1>x2+y2){
                std::swap(x1,x2);
                std::swap(y1,y2);
            }
            m_edge.insert(Edge(Pos(x1,y1),Pos(x2,y2)));
            m_pos_edge[Pos(x1,y1)].push_back(Edge(Pos(x1,y1),Pos(x2,y2)));
            m_pos_edge[Pos(x2,y2)].push_back(Edge(Pos(x2,y2),Pos(x1,y1)));
        }
    }
    // else qDebug()<<"read error!!!";
    fin.close();

}
void Map::readplace(){
    std::ifstream fin;
    fin.open("./src/place.txt",std::ios::in);
    if(fin.good()){
        std::string name;
        int x1,y1,x2,y2;
        while(!fin.eof()){
            fin>>name>>x1>>y1>>x2>>y2;
            m_place[name]=Edge(Pos(x1,y1),Pos(x2,y2));
        }
    }
    // else qDebug()<<"read error!!!";
    fin.close();
}

bool Map::AddEdge(Pos &Start,Pos &End){
    std::ofstream fout;
    fout.open("./src/edge.txt",std::ios::app);
    if(fout.bad()) fout.open("./src/edge.txt",std::ios::out);
    fout<<Start.x<<" "<<Start.y<<" ";
    fout<<End.x<<" "<<End.y<<std::endl;
    fout.close();
    m_edge.insert(Edge(Start,End));
    m_pos_edge[Start].push_back(Edge(Start,End));
    m_pos_edge[End].push_back(Edge(End,Start));
    return true;
}

bool Map::AddEPos(int x,int y){
    Pos temp(x,y);
    m_pos.insert(temp);
    if(m_pos.find(temp)!=m_pos.end()){
        temp=m_pos.find(temp).operator*();
        std::ofstream fout;
        fout.open("./src/edge_pos.txt",std::ios::app);
        if(fout.bad()) fout.open("./src/edge_pos.txt",std::ios::out);
        fout<<x<<" "<<y<<std::endl;
        fout.close();
        return true;
    }
    return false;
}

bool Map::AddPlace(std::string name,Pos pos){
    if(m_place.find(name)!=m_place.end()){
        // qDebug()<<"place have been here!!!" ;
        return false;
    }
    // m_place[name]=pos;
    Pos End;
    int dis=1e9;
    std::set<Pos>::iterator it=m_pos.begin();
    for(;it!=m_pos.end();it++){
        if(dis>dist(pos,*it)){
            dis=dist(pos,*it);
            End=*it;
        }
    }
    m_place[name]=Edge(pos,End);
    
    std::ofstream fout;
    fout.open("./src/place.txt",std::ios::app);
    if(fout.bad()) fout.open("./src/place.txt",std::ios::out);
    fout<<name<<" "<<pos.x<<" "<<pos.y<<" ";
    fout<<End.x<<" "<<End.y<<std::endl;
    fout.close();
    // AddEdge(pos,End);
    return true;
}

bool Map::ErasePlace(std::string name){
    if(m_place.find(name)==m_place.end()) return false;
    m_place.erase(name);
    std::ofstream fout;
    fout.open("./src/place.txt",std::ios::out);
    if(fout.bad()) return false;
    std::map<std::string,Edge>::iterator it;
    for(it=m_place.begin();it!=m_place.end();it++){
        fout<<it->first<<" "<<it->second.start_pos.x<<" "<<it->second.start_pos.y<<" ";
        fout<<it->second.end_pos.x<<" "<<it->second.end_pos.y<<std::endl;
    }
    fout.close();
    return true;
}

std::vector<Pos> Map::dijkstra(std::string sname,std::string ename){
    if(m_place.find(sname)==m_place.end() || m_place.find(ename)==m_place.end()){
        // qDebug()<<"there is no sname or ename";
        return std::vector<Pos>();
    }
    Edge sedge=m_place[sname];
    Edge eedge=m_place[ename];
    Pos spos=sedge.end_pos;
    Pos epos=eedge.end_pos;
    std::map<Pos,double,pos_cmp> m_dist;
    std::map<Pos,bool,pos_cmp> m_vis;
    std::map<Pos,std::vector<Pos>,pos_cmp> m_ans;
    const double MaxDist=1e5;

    std::map<Pos,std::vector<Edge>>::iterator it;
    for(it=m_pos_edge.begin();it!=m_pos_edge.end();++it){
        m_dist[it->first]=MaxDist;
        m_vis[it->first]=false;
        m_ans[it->first]=std::vector<Pos>();
    }
    m_dist[spos]=0;
    m_ans[spos].push_back(spos);
    int n=m_dist.size();
    for(int i=0;i<n;i++){
        Pos t(-1,-1);
        for(it=m_pos_edge.begin();it!=m_pos_edge.end();++it)
            if(!m_vis[it->first] && (t==Pos(-1,-1) || m_dist[t]>m_dist[it->first]))
                t=it->first;
        m_vis[t]=true;
        if(t==epos) break;
        for(auto temp : m_pos_edge[t]){
            if(m_dist[t]+temp.len<m_dist[temp.end_pos]){
                m_dist[temp.end_pos]=m_dist[t]+temp.len;
                m_ans[temp.end_pos].clear();
                m_ans[temp.end_pos].insert(m_ans[temp.end_pos].end(),m_ans[t].begin(),m_ans[t].end());
                m_ans[temp.end_pos].push_back(temp.end_pos);
            }
        }
    }
    ans_len=m_dist[epos]*1.25;
    ans_k=ans_len*50;
    return m_ans[epos];
}

std::vector<Pos> Map::best_far(std::string sname,std::string ename){
    if(m_place.find(sname)==m_place.end() || m_place.find(ename)==m_place.end()){
        // qDebug()<<"there is no sname or ename";
        return std::vector<Pos>();
    }
    Edge sedge=m_place[sname];
    Edge eedge=m_place[ename];
    Pos spos=sedge.end_pos;
    Pos epos=eedge.end_pos;
    
    ans_far_vis.clear();
    std::map<Pos,std::vector<Edge>>::iterator it;
    for(it=m_pos_edge.begin();it!=m_pos_edge.end();++it)
        ans_far_vis[it->first]=false;
    ans_far_vis[spos]=true;
    ans_far_temp.clear();
    ans_far_dist_temp=0;
    ans_far.clear();
    ans_far_dist=0;
    ans_far_temp.push_back(spos);
    ans_i=0;
    best_far(spos,epos);
    ans_len=ans_far_dist*1.25;
    ans_k=ans_len*50;
    return ans_far;
}
void Map::best_far(Pos& s,Pos& e){ 
    if(ans_i==100) return ;
    if(s==e){
        ans_i++;
        if(ans_far_dist<ans_far_dist_temp){
            ans_far.clear();
            ans_far.insert(ans_far.end(),ans_far_temp.begin(),ans_far_temp.end());
            ans_far_dist=ans_far_dist_temp;
        }
        return ;
    }
    for(auto temp : m_pos_edge[s]){
        // auto temp=m_pos_edge[s][0];
        if(ans_far_vis[temp.end_pos]) continue;
        // if(ans_far_vis[temp.end_pos]) return ;
        ans_far_vis[temp.end_pos]=true;
        ans_far_temp.push_back(temp.end_pos);
        ans_far_dist_temp+=temp.len;
        best_far(temp.end_pos,e);
        ans_far_vis[temp.end_pos]=false;
        ans_far_temp.pop_back();
        ans_far_dist_temp-=temp.len;
    }
    return ;
}
