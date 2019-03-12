//
// Created by HFD on 11/28/2017.
//
#include <iostream>
#include "dma.h"
using namespace std;

int main()
{
    baseDMA shirt("Portabelly", 8);
    lacksDMA balloon("red", "Blimpo", 4);
    hasDMA map("Mercator", "Buffalo Keys", 5);
    cout<<shirt<<endl;
    cout<<balloon<<endl;
    cout<<map<<endl;

    lacksDMA balloon2(balloon);
    hasDMA map2;
    map2 = map;
    cout << balloon2 << endl;
    cout << map2 << endl;
    cin.get();
    return 0;
}
