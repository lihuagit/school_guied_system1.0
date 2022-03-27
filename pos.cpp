#include"pos.h"

double dist(Pos s,Pos e){
    double x=abs(s.x-e.x);
    double y=abs(s.y-e.y);
    return sqrt(x*x+y*y);
}