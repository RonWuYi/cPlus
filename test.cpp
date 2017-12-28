//
// Created by HFD on 11/14/2017.
//

#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace std;
enum Days{Sat, Sun = 0, Mon, Tue, Wed, Thur, Fri};

void Prnt(Days day)
{
    if (day == 0)
    {
        cout<<"happy" <<endl;
    }
    else
        cout<<"work"<<endl;
}
int main(){
    enum Fruit{apple, pear, orange, banana}frt1;
    typedef enum Fruit ShuiGuo;
    enum Fruit frt2 = apple;
    ShuiGuo frt3 = pear;
    frt1 = (Fruit)0;
    for (int i = apple; i <=banana ; i++)
        switch (i)
    {
        case apple:cout<<"apple" <<endl;
            break;
        case pear:cout<<"pear" <<endl;
            break;
        case orange:cout<<"orange" <<endl;
            break;
        case banana:cout<<"banana" <<endl;
            break;
//
//        case apple:cout<<"apple" <<endl;
//            break;
    }

    Prnt(Sat);
    Prnt(Sun);
    Prnt(Mon);
    Prnt(Tue);
    Prnt(Thur);
    Prnt(Fri);
    Prnt(Wed);
    cin.get();
    return 0;
}