//
// Created by HFD on 10/26/2017.
//
//#include ""
#include <iostream>

void cheers(int);
double cube(double x);
using namespace std;

int main(void)
{
    cheers(5);
    cout << "give a number ";
    double side;
    cin >> side;
    double volum = cube(side);
    cout << "A" << side << " - foot cube has a volume of ";
    cout << volum << " cubic feet.\n";
    cheers(cube(2));
    cin.get();
    cin.get();
    return 0;
}
void cheers(int n)
{
    for (int i = 0; i < n ; i++) {
        cout << "Cheers! ";
    }
    cout << endl;
}

double cube(double x)
{
    return x*x*x;
}