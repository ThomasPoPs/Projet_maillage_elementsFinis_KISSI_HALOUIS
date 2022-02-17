#include<iostream>
#include<fstream>
#include<vector>
#include <cmath>
#include <IterativeLinearSolvers>
#include <SparseCore>
#include "Mesh.hpp"

#ifndef PbEF2d_HPP
#define PbEF2d_HPP

typedef Eigen::SparseMatrix<double> SparseMatrix; 
typedef Eigen::Triplet<double> Triplet;
typedef Eigen::VectorXd Vecteur;

#define DIRICHLETFROID 0
#define DIRICHLETCHAUD 1 
#define NEUMANN        2
int FROID  = 0;
int CHAUD  = 0;

class PbEF2d{
    public: 
        Mesh mesh; 
        SparseMatrix A; 
        Vecteur X, F; 

        PbEF2d(std::string filename);  
        int l2g(Triangle T, int i); 
        void assemblage(); 
        void solve(); 
        void Export(); 
        double calcul_erreur(); 
        double f(double x, double y); 
        double g(Point p); 
}; 

PbEF2d::PbEF2d(std::string filename) : mesh(filename), A(mesh.N, mesh.N), F(mesh.N), X(mesh.N){
    // std::cout << mesh << std::endl;
    if(filename.find("appart") != std::string::npos){
        CHAUD = 20;
        FROID = 10;
    }
}


/** Fonction l2g (local to global) : qui à une maille T et un indice local de sommet 
associe l'indice global du degré de liberté correspondant. **/

int PbEF2d::l2g(Triangle T, int i){
    return T.get_sommet(i).degre_lib; 
}

void PbEF2d::assemblage(){
    
    std::vector<Triplet> coefficients;   
    for(int i = 0; i < mesh.N; i++)
        F(i) = 0; 
        
    for(auto& T : mesh.list_triangle){
        for(int i = 0; i < 3; i++){
            int I = l2g(T, i); 
            if(I == -1)
                continue; 
            double s = 0; 

            for(int k = 0; k <3; k++){
                auto x_y = T.milieu_arete(k); 
                double x = std::get<0>(x_y); 
                double y = std::get<1>(x_y); 
                s += f(x, y)*T.phi(x, y, i); 
            }
             
            F(I) += (T.air_triangle()*s)/3.; 
            for(int j = 0; j < 3; j++){
                int J = l2g(T, j); 
                std::vector<double> grad_phi_i = T.grad_phi(i); 
                std::vector<double> grad_phi_j = T.grad_phi(j); 
                if(J == -1)
                    F(I) -= g(T.get_sommet(j))*T.air_triangle()*(produit_scalaire(grad_phi_i, grad_phi_j)); 
                else
                    coefficients.push_back(Triplet(I, J, T.air_triangle()*(produit_scalaire(grad_phi_i, grad_phi_j)))); 
            }

        }
    }
    A.setFromTriplets(coefficients.begin(), coefficients.end());
}


void PbEF2d::solve(){
    Eigen::ConjugateGradient<Eigen::SparseMatrix<double>, Eigen::Lower|Eigen::Upper> cg;
    cg.compute(A); 
    X = cg.solve(F); 
}

void PbEF2d::Export(){
    std::string const exportParaview("resultatParaview.txt");
    std::ofstream paraview(exportParaview.c_str());
    if(paraview){    
        paraview << "x,y,val" << std::endl;
        for(auto& p : mesh.list_point){
            if(p.in_edge == DIRICHLETFROID)
                paraview << p.x << "," << p.y << ","  << FROID << std::endl; 
            else if(p.in_edge == DIRICHLETCHAUD)
                paraview << p.x << "," << p.y << ","  << CHAUD << std::endl; 
            else
                paraview << p.x << "," << p.y << "," << X[p.degre_lib] << std::endl; 
        }
    }
    else
        std::cout << "ERREUR: Impossible d'ouvrir le fichier." << std::endl;

    std::string const exportGNUPLOT("resultatGNUPLOT.txt");
    std::ofstream gnuplot(exportGNUPLOT.c_str());
    if(gnuplot){    
        for(auto& p : mesh.list_point){
            if(p.in_edge == DIRICHLETFROID)
                gnuplot << p.x << " " << p.y << " "  << FROID << std::endl; 
            else if(p.in_edge == DIRICHLETCHAUD)
                gnuplot << p.x << " " << p.y << " "  << CHAUD << std::endl; 
            else
                gnuplot << p.x << " " << p.y << " " << X[p.degre_lib] << std::endl; 
        }
    }
    else
        std::cout << "ERREUR: Impossible d'ouvrir le fichier." << std::endl;
}


double u(Point p){
    return sin(M_PI* p.x)*cos(M_PI*p.y); 
}
// Carré
// double laplacien_u(double x, double y){
//     return 2*M_PI*M_PI*sin(M_PI* x)*cos(M_PI*y); 
// }
// double PbEF2d::f(double x, double y){
//     return laplacien_u(x, y); 
// }
// double PbEF2d::g(Point p){
//     return 0; 
// }

// Appartement 
double PbEF2d::f(double x, double y){
    return 0; 
}
double PbEF2d::g(Point p){
    if(p.in_edge == 0)
        return FROID;
    else if (p.in_edge == 1)
        return CHAUD; 
    return 0; 
}

double PbEF2d::calcul_erreur(){
    double erreur; 
    double er2 = 0;

    for(auto& T : mesh.list_triangle){
        std::vector<double> delta_u{0, 0, 0}; 

        for(int i = 0; i < 3; i++){
            int I = l2g(T, i); 
            if(I == -1)
                continue; 
            delta_u[i] = u(T.get_sommet(i)) - X[I]; 
        }
        double somme = 0; 

        std::vector<double> delta_gradphi{0, 0}; 
        for(int i = 0; i < 3; i++){
            std::vector<double> new_delta_gradphi = produit_scalaire_vect(delta_u[i], T.grad_phi(i)); 
            delta_gradphi = add_two_vect(delta_gradphi, new_delta_gradphi); 
        }

        er2 += T.air_triangle() * produit_scalaire(delta_gradphi, delta_gradphi); 
    }

    erreur = sqrt(er2); 

    return erreur; 
}


#endif 