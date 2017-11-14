//
// Created by Ron on 11/14/2017.
//
#include <iostream>
using std::cout;
#include "strngbad.h"

void callme1(StringBad &);
void callme2(StringBad);

int main(){
    using std::endl;
    StringBad headline1("Celery Stalks at Midnight");
    StringBad headline2("Lettuce Prey");
    StringBad sports("Spinach Leaves Bowl for Dollars");
    cout << "headline1: " << headline1 << endl;
    cout << "headline2: " << headline2 << endl;
    cout << "sports: " << sports << endl;
    callme1(headline1);
    cout << "headline1: " << headline1 << endl;
    callme2(headline2);
    cout << "headline1: " << headline2 << endl;
    cout <<"Initialize one object to another: \n";
    StringBad sailor = sports;
    cout << "sailor: " << sailor << endl;
    cout << "assign one object to another: \n";
    StringBad knot;
    knot = headline1;
    cout << "knot: " << knot << endl;
    cout << "end of main()\n";

    std::cin.get();
    return 0;
}

void callme1(StringBad & rsb){
    cout << "String passed by reference: \n";
    cout << "\"" <<rsb<<"\"\n";
}

void callme2(StringBad rsb){
    cout << "String passed by value: \n";
    cout << "\"" <<rsb<<"\"\n";
}