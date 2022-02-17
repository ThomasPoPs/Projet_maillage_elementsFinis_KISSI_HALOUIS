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
    string filename = "appart.msh"; 
    PbEF2d p(filename);
    p.assemblage(); 
    p.solve(); 
    p.Export(); 
   return 0; 
}