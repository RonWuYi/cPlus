//
// Created by Ron on 11/14/2017.
//
#include <iostream>
using std::cout;
#include "stonewt.h"
void display(const Stonewt st, int n);

int main(){
//    Stonewt pavarotti = 260;
//    Stonewt wolfe(285.7);
//    Stonewt taft(21, 8);
    Stonewt poppins(9, 2.8);
    double p_wt = poppins;
    cout << "to d - ";
    cout << "Poppins: " << p_wt << " pounds.\n";
    cout << "to int - ";
    cout << "Poppins: " << int(p_wt) << " pounds.\n";

    std::cin.get();
    return 0;
}