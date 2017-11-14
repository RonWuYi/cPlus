//
// Created by HFD on 10/31/2017.
//
#include <iostream>

double betsy(int);
double pam(int);
void estimate(int lines, double(*pf)(int));
using namespace std;

int main(){
    int code;
    cout << "input lines ";
    cin >> code;
    cout<< "estimate from betsy:\n";
    estimate(code, betsy);
    cout<< "estimate from pam:\n";
    estimate(code, pam);

    cin.get();
//    cin.get();
    cin.get();
    cin.get();
    return 0;
}

double betsy(int lns){
    return 0.05*lns;
}

double pam(int lns){
//    return (0.03*lns) + (0.004*lns*lns);
    return 0.03*(lns + 0.004)*lns*lns;
}

void estimate(int lines, double(*pf)(int)){
    cout << lines << " lines will take ";
    cout << (*pf)(lines) << " hour(s) \n";
}