//
// Created by HFD on 11/17/2017.
//
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "string1.h"
const int ArSize = 10;

const int MaxLen = 81;

int main()
{
    using namespace std;
    String name;

    cout<< "input name from saying2\n>>";
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
    if(total > 0)
    {
        cout << "what are you sayings:\n";
        for (int i = 0; i < total; i++) {
            cout << saying[i][0] <<" : " <<saying[i] << endl;
        }

        // use pointers
        String * shortest = &saying[0];
        String * first = &saying[0];

        for (i = 1; i < total; i++) {
            if(saying[i].length()< shortest->length())
                shortest = &saying[i];
            if(saying[i] < *first)
                first = &saying[i];
        }
        cout << "Shortest:\n" << * shortest << endl;
        cout << "First:\n" << * first << endl;

        srand(time(0));
        int choice = rand()%total;
        // use new to create
        String * favorite = new String(saying[choice]);
        cout << "My favorite: \n" << * favorite << endl;
        delete favorite;
    } else
        cout << "No much to say, eh?\n";
    cout << "Bye.\n";

    cin.get();
    return 0;

}
