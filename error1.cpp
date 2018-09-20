//
// Created by hdc on 12/28/17.
//
#include <iostream>
#include <cfloat>


using namespace std;
double hmean(double a, double b);

int main()
{
    double x, y, z;
    cout << "enter two numbers: ";
    while (cin >> x >> y)
    {
        try {
            z = hmean(x, y);
        }
        catch (const char *s)
        {
            cout << s <<endl;
            cout << "enther a new pair of numbers: ";
            continue;
        }
        cout << "Harmonic mean of " << x << " and " << y
            << " is " << z <<endl;
        cout << "enter next numbers <q to quit>";
        cout << endl;
    }
    cout << "Bye!\n";
    cin.get();
    return 0;
}

double hmean(double a, double b)
{
    if(a == -b)
    {
        throw "bad arguments not allowed";
    }
    return 2.0*a*b/(a+b);
}
