#include<iostream>
#include<fstream>
#include<vector>
#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

std::vector<double> add_two_vect(std::vector<double> v1, std::vector<double> v2)
{
    std::vector<double> resultat; 
    resultat.push_back(v1[0]+v2[0]); 
    resultat.push_back(v1[1]+v2[1]); 

    return resultat;
}


double produit_scalaire(std::vector<double> v1, std::vector<double> v2){
    return v1[0]*v2[0] + v1[1]*v2[1]; 
}

std::vector<double> produit_scalaire_vect(double d, std::vector<double> v){
    std::vector<double> resultat; 
    resultat.push_back(d*v[0]); 
    resultat.push_back(d*v[1]); 

    return resultat;
}


class Triangle{
    public: 
        /** Attributs **/
        int id; 
        Point s1; 
        Point s2; 
        Point s3; 

        /** Méthodes **/
        Triangle(int id, Point s1, Point s2, Point s3); 
        friend std::ostream& operator<<(std::ostream& out, Triangle& triangle);
        double air_triangle(); 
        double phi(double x, double y, int i); 
        std::vector<double> grad_phi(int i); 
        std::tuple<double, double> milieu_arete(int k); 
        Point get_sommet(int i); 
}; 

Triangle::Triangle(int id, Point s1, Point s2, Point s3){
    this->id = id; 
    this->s1 = s1; 
    this->s2 = s2; 
    this->s3 = s3; 
}

std::ostream& operator<<(std::ostream& out, Triangle& triangle)
{
    out << triangle.s1 << std::endl; 
    out << triangle.s2 << std::endl; 
    out << triangle.s3 << std::endl; 
    return out;
}



double Triangle::air_triangle(){
    return ((s3.x-s2.x)*(s1.y-s3.y)-(s3.y-s2.y)*(s1.x-s3.x))/2; 
}

Point Triangle::get_sommet(int i){
    Point reponse; 
    switch (i)
    {
    case 0:
        reponse = s1; 
        break;
    case 1:
        reponse = s2; 
        break; 
    case 2: 
        reponse = s3; 
        break; 
    }
    return reponse; 
}
std::tuple<double, double> Triangle::milieu_arete(int k){
    double x, y; 
    switch (k)
    {
    case 0:
    {
        x = (s1.x+s2.x)/2; 
        y = (s1.y+s2.y)/2; 
        break;
    }
    case 1: 
    {
        x = (s2.x+s3.x)/2; 
        y = (s2.y+s3.y)/2; 
        break;
    }
    case 2:
    {
        x = (s3.x+s1.x)/2; 
        y = (s3.y+s1.y)/2; 
        break;
    }
    }
    return std::make_tuple(x, y);
}


double Triangle::phi(double x, double y, int i){
    double deno = ((s3.x-s2.x)*(s1.y-s3.y)-(s3.y-s2.y)*(s1.x-s3.x)); 
    double num; 
    switch (i)
    {
    case 0:
    {
        num = (s3.x-s2.x)*(y-s3.y)-(s3.y-s2.y)*(x-s3.x);  
        break;
    }
    case 1:
    {
        num = ((s3.x-x)*(s1.y-s3.y)-(s3.y-y)*(s1.x-s3.x));
        break; 
    }
    case 2: 
    { 
        num = (x-s2.x)*(s1.y-y)-(y-s2.y)*(s1.x-x); 
        break; 
    }
    }
    return num/deno;
}


std::vector<double> Triangle::grad_phi(int i){
    std::vector<double> grad; 
    double deno = ((s3.x-s2.x)*(s1.y-s3.y)-(s3.y-s2.y)*(s1.x-s3.x)); 
    double num; 
    switch (i)
    {
    case 0:
    {
        grad.push_back(-(s3.y-s2.y)/deno); 
        grad.push_back((s3.x-s2.x)/deno); 
        break;
    }
    case 1:
    {
        grad.push_back(-(s1.y-s3.y)/deno); 
        grad.push_back((s1.x-s3.x)/deno); 
        break; 
    }
    case 2: 
    { 
        grad.push_back((s1.y-s2.y)/deno); 
        grad.push_back((s2.x-s1.x)/deno); 
        break; 
    } 
    }
    return grad; 
}

#endif
