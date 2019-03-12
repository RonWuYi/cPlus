//
// Created by HFD on 11/7/2017.
//
#include <iostream>
#include "coordin.h"

using namespace std;

int main(){
    rect rplace;
    polar pplace;
    cout << "enter x and y values: ";
    while (cin >> rplace.x >> rplace.y)
    {
        pplace = rect_to_polar(rplace);
        show_polar(pplace);
        cout << "net two numbers(q to quit): ";
    }
    cout << "Bye!\n";
    return 0;
}