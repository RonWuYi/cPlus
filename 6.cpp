//
// Created by HFD on 10/31/2017.
//
#include <iostream>

//double cube(double a);
double refcube(const double &ra);
using namespace std;

int main(){
//    double x = 3.0;
//
//    cout << cube(x);
//    cout << " = cube of " << x << endl;
//
//    cout << refcube(x);
//    cout << " = cube of " << x << endl;

    double side = 3.0;
    double *pd = &side;
    double & rd = side;
    long edge = 3L;
    double lens[4] = {2.0, 5.0, 3.0, 12.0};

    double c1 = refcube(side);
    double c2 = refcube(lens[2]);
    double c3 = refcube(rd);
    double c4 = refcube(*pd);
    long c5 = refcube(edge);
    double c6 = refcube(3.0);
    double c7 = refcube(side + 0);

    cout << c1 <<endl;
    cout << c2 <<endl;
    cout << c3 <<endl;
    cout << c4 <<endl;
    cout << c5 <<endl;
    cout << c6 <<endl;
    cout << c7 <<endl;


    cin.get();
    return 0;
}
//double cube(double a){
//    a *= a*a;
//    return a;
//}

//double refcube(double &ra){
//    ra *= ra * ra;
//    return ra;
//}
double refcube(const double &ra){
    return ra*ra*ra;
}