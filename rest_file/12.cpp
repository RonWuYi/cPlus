//
// Created by HFD on 11/7/2017.
//
#include <iostream>

using namespace std;
int tom = 3;
int dick = 30;
static int harry = 300;
void remote_access();
//double warming = 0.3;
//void update(double dt);
//void local();

int main(){
    cout << "main() reports the following addressed: \n";
    cout << &tom << " = &tom " << &dick << " = &dick, ";
    cout << &harry << " = &harry\n";
    remote_access();
    cin.get();
    return 0;

}

//void update(double dt){
//    extern double warming;
//    warming += dt;
//    cout << "updating to " << warming;
//    cout << " degress.\n";
//}
//
//void local(){
//    double warming = 0.8;
//    cout << "local = " << warming << " degress.\n";
//    cout << "but global warming is " << ::warming;
//    cout << " degress.\n";
//}