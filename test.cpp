//
// Created by HFD on 11/14/2017.
//

#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace std;
//bool newcustomer(double x){
//    return (std::rand()*x/RAND_MAX < 1);
//}
int main(){
    std::srand(std::time(0));
    cout << rand() <<endl;
    cin.get();
    return 0;
}