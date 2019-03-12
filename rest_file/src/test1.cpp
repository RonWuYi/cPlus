//
// Created by hdc on 11-1-18.
//
#include <iostream>
#include <string>
#include <cstring>
//const double Inch_per_feet = 12.0L;
//const int Strlen = 20;


int main()
{
    using namespace std;
//
//    struct CandyBar
//    {
//        std::string CBrand;
//        float CPang;
//        int Car;
//    };
//
//
//    CandyBar *snack =new CandyBar [3];
//
//
//    for (int i = 0; i < 3 ; ++i) {
//        snack[i] = {"Mocha Munch1", 2.31, 4001};
//    }
////    CandyBar snack[3]={
////            {"Mocha Munch", 2.3, 400},
////            {"Mocha Munch1", 2.31, 4001},
////            {"Mocha Munch2", 2.32, 4002}
////    };
//    for (int j = 0; j < 3; ++j) {
//        cout << snack[j].CPang << endl;
//        cout << snack[j].Car << endl;
//        cout << snack[j].CBrand << endl;
//        cout << "#############################" <<endl;
//    }
////    snack.CPang = 2.3;
////    snack.Car = 350;
////
////    cout << snack << endl;
    string test;
    cin >> test;
    char namm1[5] = "c++";

    cout << sizeof(test) << endl;
    cout << sizeof(namm1) <<endl;
//    cout << strlen(test) << endl;
    cout <<strlen(namm1) <<endl;
//    cout << "name - " << snack->CBrand  <<":"<<snack->CPang<<endl;
    return 0;
}
