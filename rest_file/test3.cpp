//
// Created by hdc on 12-1-18.
//
#include <iostream>
#include <string>
#include <cstring>
struct test
{
    std::string brand;
};
int main()
{
    using namespace std;
//    cout << "build year?\n";
//    int year;
//    cin >>  year;
//    cin.get();
//    cout << "address?\n";
//    char address[80];
//    cin.getline(address, 80);
//    cout <<"year is "<<year <<endl;
//    cout <<"address "<<address <<endl;
//    cout <<"done!\n";
    test *test1 = new test[2];

    for (int i = 0; i < 2; ++i) {
        test1[i] = {"name"};
    }

    for (int i = 0; i < 2; ++i) {
        cout << test1[i].brand <<endl;
    }
    delete []test1;
    return 0;
}
