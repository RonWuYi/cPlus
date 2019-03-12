//
// Created by HFD on 11/7/2017.
//

#include <iostream>
extern int tom;
static int dick = 20;
int harry = 200;
using namespace std;
void remote_access()
{
    cout << "remote() reports the following addressed: \n";
    cout << &tom << " = &tom " << &dick << " = &dick, ";
    cout << &harry << " = &harry\n";
}