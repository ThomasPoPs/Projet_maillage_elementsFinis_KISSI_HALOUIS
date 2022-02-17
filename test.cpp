#include<iostream>
#include<fstream>
#include<vector> 
#include<iomanip>
#include<sstream>
#include "Point.hpp"
#include "Triangle.hpp"
#include "Mesh.hpp"
#include "PbEF2d.hpp"

using namespace std;

int main()
{   
    string filename = "square0_01.msh"; 
    PbEF2d p0(filename);
    p0.assemblage(); 
    p0.solve(); 
    p0.Export();
    cout << "Erreur = "<< p0.calcul_erreur()<< " (h = 0.01)" <<endl; 

    filename = "square0_005.msh"; 
    PbEF2d p1(filename);
    p1.assemblage(); 
    p1.solve(); 
    cout << "Erreur = "<< p1.calcul_erreur()<< " (h = 0.005)" <<endl; 
    p1.Export(); 

    filename = "square0_0025.msh"; 
    PbEF2d p2(filename);
    p2.assemblage(); 
    p2.solve(); 
    cout << "Erreur = "<< p2.calcul_erreur() << " (h = 0.0025)" <<endl; 
    return 0; 
}