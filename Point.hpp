#include<iostream>
#include<fstream>
#include<vector> 
#ifndef POINT_HPP
#define POINT_HPP

class Point{
    public: 
        /** Attributs **/
        int                 id;
        double              x, y; 
        int                 in_edge = 3; 
        int                 degre_lib = -2;  

        /** Méthodes **/
        Point(int id = -1, double x = -1 , double y = -1); 
        bool on_edge();
        bool isDirichlet();

        Point& operator=(const Point &p);
        friend std::ostream& operator<<(std::ostream& out, Point& point);
}; 

Point::Point(int id, double x, double y){
    this->id = id; 
    this->x = x; 
    this->y = y; 
}

Point& Point::operator=(const Point &p){ 
    id = p.id;
    x = p.x;
    y = p.y;
    in_edge = p.in_edge;
    degre_lib = p.degre_lib; 
    return *this; 
}

bool Point::on_edge(){
    return in_edge != 3;
}

bool Point::isDirichlet(){
    return in_edge == 0 or in_edge == 1;
}

std::ostream& operator<<(std::ostream& out, Point& point)
{
    out << "ID = " << point.id << " ("  << point.x << ','  << point.y << ")" << " ddl = "<< point.degre_lib << '\t'; 
    if(point.in_edge == 0)
        out << " isColdDirichlet"; 
    if(point.in_edge == 1)
        out << " isHotDirichlet"; 
    if(point.in_edge == 2)
        out << " Neumann"; 
    return out;
}

#endif