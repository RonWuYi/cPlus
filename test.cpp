//
// Created by HFD on 11/14/2017.
//

#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace std;
bool newcustomer(double x){
    return (std::rand()*x/RAND_MAX < 1);
}
int main(){
    int total = 0;
    for (double i = 1.0; i < 61.0; i++) {
        cout << newcustomer(i) << "  during i = "<< i <<" rand()= " <<rand()<<endl;

        if(newcustomer(i)==1)
            total++;
    }

    cout << "total = "<<total << endl;

    cout << "RAND_MAX = "<<RAND_MAX << endl;
    cout << "srand(std::time(0)) = "<< srand(std::time(0)) << endl;
    cin.get();
    return 0;
}