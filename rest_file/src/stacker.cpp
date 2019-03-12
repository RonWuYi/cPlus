//
// Created by HFD on 11/10/2017.
//
#include <iostream>
#include <cctype>
#include "stack.h"

int main(){
    using namespace std;
    Stack st;
    char ch;
    unsigned long po;
    cout << "Pleaes enter A to add a purchase order, \n"
         << "P to process a PO, or Q to quit.\n";
    while (cin >> ch && toupper(ch) != 'Q')
    {
        while (cin.get()!='\n')
            continue;
        if(!isalpha(ch))
    }
}
