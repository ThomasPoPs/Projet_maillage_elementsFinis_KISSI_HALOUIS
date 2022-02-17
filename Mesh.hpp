#include<iostream>
#include<fstream>
#include<vector> 
#include<sstream>
#include <string>
#include <cassert>
#include "Point.hpp"
#include "Triangle.hpp"
#ifndef MESH_HPP
#define MESH_HPP

class Mesh{ 
    public: 
        /** Attributs */
        std::vector<Triangle> list_triangle; 
        std::vector<Point> list_point; 
        unsigned int N = 0; 

        /** Méthodes **/
        Mesh(std::string); 
        std::vector<std::string> split(const std::string &s, char delim); 
        bool in_list(std::vector<std::string> v, std::string word);
        void init(std::string filename);
        void extractNodes(std::ifstream &fichier, std::string &ligne);
        void extractTriangles(std::ifstream &fichier, std::string &ligne);
}; 

Mesh::Mesh(std::string filename){
    init(filename);
}

void Mesh::init(std::string filename) {
    std::ifstream fichier(filename, std::ios::in);
    if(fichier) {
        std::string ligne; 
        extractNodes(fichier, ligne); 
        extractTriangles(fichier, ligne); 
    }
    else {
        std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
        exit(1); 
    }
}

void Mesh::extractTriangles(std::ifstream &fichier, std::string &ligne){
    do{
        std::vector<std::string> element = split(ligne, ' ');
        unsigned int id = stoi(element[0]); 
        Point s1 = list_point[stoi(element[5]) - 1]; 
        Point s2 = list_point[stoi(element[6]) - 1]; 
        Point s3 = list_point[stoi(element[7]) - 1]; 
        list_triangle.push_back(Triangle(id, s1, s2, s3));  
        getline(fichier, ligne); 
    }while(ligne != "$EndElements"); 
}

void Mesh::extractNodes(std::ifstream &fichier, std::string &ligne){
    /** On boucle jusqu'à arriver au début de la liste des Nodes **/
    while(getline(fichier, ligne)) {
        if(ligne == "$Nodes"){
            getline(fichier, ligne);
            break;
        }
    }
    unsigned int n_nodes = stoi(split(ligne, ' ')[0]);
    for (unsigned int i = 0; i < n_nodes; ++i){
        getline(fichier, ligne);
        std::vector<std::string> coords = split(ligne, ' ');
        Point point = Point(i+1, stod(coords[1]), stod(coords[2]));  
        list_point.push_back(point);
    }

    for (int i = 0; i < 3; ++i)
        getline(fichier, ligne); 

    while(true){
        std::vector<std::string> element;
        getline(fichier, ligne); 
        element = split(ligne, ' ');
        if(stoi(element[1]) != 1)
            break;
        unsigned int boundary = stoi(element[3]); 
        assert(boundary >= 0 and boundary < 3); 
        unsigned int idPoint1 = stoi(element[5]);
        unsigned int idPoint2 = stoi(element[6]); 
        list_point[idPoint1 - 1].in_edge = boundary; 
        list_point[idPoint2 - 1].in_edge = boundary; 
    }

    /** Initialisation de l'indice global du degré de liberté **/ 
    for(auto& p : list_point)
        if(p.isDirichlet())
            p.degre_lib = -1; 
        else
            p.degre_lib = N++; 
}

std::ostream& operator<<(std::ostream& out, Mesh& mesh)
{
    out << "Points du maillage" << std::endl; 
    for(Point &p : mesh.list_point) 
        out << p << std::endl; 
    out << std::endl;

    out << "Triangles du maillage" << std::endl; 
    for(Triangle &t : mesh.list_triangle) 
        out << t << std::endl; 
    return out;
}

std::vector<std::string> Mesh::split(const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (getline (ss, item, delim))
        result.push_back(item);
    
    return result;
}

bool Mesh::in_list(std::vector<std::string> v, std::string word){
    bool resultat = false; 
    for (size_t i =0; i < v.size(); i++){
        if(v[i] == word){
            resultat = true; 
        }
    }
    return resultat; 
}

#endif