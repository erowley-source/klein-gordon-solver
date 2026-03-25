#include <iostream>
#include <eigen3/Eigen/Dense>
#include <fstream>
#include <string>
#include <iomanip>
#include <complex>
#include <math.h>
using namespace Eigen;
using namespace std;
using namespace std::complex_literals;

MatrixXcd grad_1d(int n);
double gauss(double x, double m, double s);
MatrixXcd paroi(double L, int N, double V0, double a, double b);
void ecrit_new(string filename, VectorXcd u);
void ecrit(string filename, VectorXcd u);

int main(){
    double dt = 0.05; // discretization du temps
    double L = 10; // taille de la boite
    int N = 200; // nombre de divisions
    double dx = L/N; // separation entre les points du reseau
    double tf = 800; // duree de la simulation (s)
    double h_bar = 1; // constante de Planck reduite
    double m = 1; // masse de la particule
    VectorXcd u0 = VectorXcd(N+1); // configuration initiale, obtenue en diagonalisant le Hamiltonien
    for (int k = 0; k<=N; k++){ // initialisation du vecteur à un profil gaussien, signature (x, mu, sigma)
        if(k == 0 || k == N){
            u0[k] = 0;
        }
        else{
            u0[k] = gauss(k*dx,3*L/4,L/50);
        }
    }
    ecrit_new("solution_temporelle.dat",u0);
    std::cout << paroi(L,N,30,4,6) << endl;
    VectorXcd unew = u0; // enregistrons ici l'etat actuel de la fonction d'onde
    MatrixXcd H = dt/(2*h_bar)*grad_1d(N+1) + dt/(2i*h_bar)*paroi(L,N,0.4,4.5,5.5) ;//opérateur différentiel qui représente le Hamiltonien
    // parioi(L,N,V,a,b) représente une barrière de potentiel V fini entre a et b
    MatrixXcd H_m = MatrixXcd::Identity(N+1,N+1) + H; // construisons l'operateur differentiel lineaire I-H
    MatrixXcd H_p = MatrixXcd::Identity(N+1,N+1) - H; //operateur differentiel I+H
    // std::cout << H_p*H_p << endl; // on observe que pour dt petit, Hp-1 ~ I
    // boucle principale pour implementer Crank-Nicolson
    MatrixXcd M = H_p.inverse()*H_m; // Calculons la matrice d'évolution temporelle
    for(double t=0; t<tf; t+=dt){
    std::cout << tf << " " << t << endl;
        //unew = H_p*(H_m*unew);
        //unew = H_p.colPivHouseholderQr().solve(H_m*unew);
        unew = M*unew;
        //imposons les conditions au bords
        unew[0] = 0;
        unew[N] = 0;
        if (int(fmod(t/dt,10))==0){
            ecrit("solution_temporelle.dat",unew);
        }
        std::cout << int(fmod(t/dt,100)) << endl;
    }
    std::cout << unew << endl;

}

MatrixXcd paroi(double L, int N, double V0, double a, double b){
    MatrixXcd V = MatrixXcd::Zero(N+1,N+1);
    double dx = L/N;
    for(int j=0; j < N + 1; j++){
        if(dx*j >= a && dx*j <= b){
            V(j,j) = V0;
        }
    }
    return V;
}

MatrixXcd grad_1d(int n){
    MatrixXcd mat = MatrixXcd::Zero(n,n);
    // boucle principale
    for (int k = 0; k < n-1; k++){
        mat(k,k) = -2i;
        mat(k,k+1) = 1i;
        mat(k+1,k) = 1i;
    }
    mat(n-1,n-1) = -2;
    return mat;
}

double gauss(double x, double m, double s){
    double coeff = 1/(sqrt(2*M_PI)*s);
    return coeff*exp(-pow(x-m,2)/(2*pow(s,2)));
}

void ecrit_new(string filename, VectorXcd u){
    ofstream file;
    file.open(filename, ios::out);
    if (! file){
        cerr << "Impossible d'ouvrir " << filename << endl;
        exit(EXIT_FAILURE);
    }

    file << setiosflags(ios::scientific) << setprecision(7);
    for (int i = 0; i<size(u); i++){
        file << u(i) << " ";
    }
    file << endl;
    file.close();
}

void ecrit(string filename, VectorXcd u){
    ofstream file(filename, ios_base::app | ios_base::out);
    //ofstream file;
    //file.open(filename, ios::out);
    if (! file){
        cerr << "Impossible d'ouvrir " << filename << endl;
        exit(EXIT_FAILURE);
    }

    file << setiosflags(ios::scientific) << setprecision(7);
    for (int i = 0; i<size(u); i++){
        file << u(i) << " ";
    }
    file << endl;
    file.close();
}
/*def gauss(x,s,m):
    a = 1/(np.sqrt(2*np.pi)*s)
    return a*np.exp(-pow(x-m,2)/(2*pow(s,2)))*/
