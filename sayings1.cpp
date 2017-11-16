//
// Created by HFD on 11/16/2017.
//
#include <iostream>
#include "string1.h"
const int ArSize = 10;
const int MaxLen = 81;

int main()
{
    using std::cout;
    using std::cin;
    using std::endl;
    String name;
    cout<< "input name\n>>";
    cin >> name;
    cout << name << " , please enter up to " << ArSize
                                             <<" short sayings <empty line to quit>:\n";
    String saying[ArSize];
    char temp[MaxLen];
    int i;
    for (i = 0; i < ArSize; i++) {
        cout << i+1 << ": ";
        cin.get(temp, MaxLen);
        while (cin && cin.get() != '\n')
            continue;
        if(!cin || temp[0] == '\0')
            break;
        else
            saying[i] = temp;
    }

    int total = i;
    cout << "what yousayings:\n";
    for (int i = 0; i < total; i++) {
        cout << saying[i][0] <<" : " <<saying[i] << endl;
    }
    int shortest = 0;
    int first = 0;
    for (int i = 1; i < total; i++) {
        if(saying[i].length() < saying[shortest].length())
            shortest = i;
        if(saying[i] < saying[first])
            first = i;
    }

    cout << "Shortest:\n" << saying[shortest] << endl;
    cout << "First:\n" << saying[first] << endl;
    cout << "Program used " << String::HowMany()
                            << " String objects. Bye. \n";
    cin.get();
    return 0;
}
