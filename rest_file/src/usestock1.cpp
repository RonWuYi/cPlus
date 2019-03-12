//
// Created by HFD on 11/9/2017.
//
#include <iostream>
#include "stock1.h"

int main(){
    using std::cout;
    using std::ios_base;
//    Stock stock1;
//    stock1.acquire("NanoSmart", 20, 12.50);
    cout.precision(2);
    cout.setf(ios_base::fixed, ios_base::floatfield);
    cout.setf(ios_base::showpoint);

    cout << "using constructors to create new objects\n";

    Stock stock1("NanoSmart", 12, 20.0);
    stock1.show();
    Stock stock2 = Stock("Boffo Objects", 2, 2.0);
    stock2.show();

    cout<<"assigning stock1 to stock2:\n";
    stock2 = stock1;
    cout<<"lising stock1 and stock2:\n";
    stock1.show();
    stock2.show();

    cout << "using constructors to reset an objects\n";
    stock1 = Stock("Nifty foods", 10, 50.0);
    cout << "Revised stock1: \n";
    stock1.show();
    cout << "Done\n";
    std::cin.get();
    return 0;
}
