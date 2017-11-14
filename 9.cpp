//
// Created by HFD on 11/6/2017.
//
#include <iostream>

using namespace std;
template <class Any>
void Swap(Any &a, Any &b);

struct job{
    char name[40];
    double salary;
    int floor;
};

template <>void Swap<job>(job &j1, job &j2);
//void Show(int a[]);
void Show(job &j);
//const int Lim = 8;

int main(){
    cout.precision(2);
    cout.setf(ios::fixed, ios::floatfield);
    int i = 10;
    int j = 20;
    cout <<"i, j = " << i << ". " << j << ".\n";
    cout <<"using template int swapper: \n";
    swap(i, j);
    cout <<"Now i, j = " << i << ". " << j << ".\n";

    job sue = {"Susan", 73000.60, 7};
    job sidney = {"sidney", 78600.70, 9};

//    double x = 24.5;
//    double y = 81.7;
//    cout <<"x, y = " << x << ". " << y << ".\n";
//    cout <<"using template swapper: \n";
//    swap(x, y);
//    cout <<"Now x, y = " << x << ". " << y << ".\n";

    cin.get();
    cin.get();
    return 0;
}

template <class Any>
void Swap(Any &a, Any &b){
    Any temp;
    temp = a;
    a = b;
    b = temp;
}

template <>void Swap<job> (job &j1, job &j2){
    double t1;
    int t2;
    t1 = j1.salary;
    j1.salary = j2.salary;
    j2.salary = t1;
    t2 =j1.floor;
    j1.floor = j2.floor;
    j2.floor = t2;
};

void Show(job & j){
    cout << j.name << j.salary << j.floor;
    cout << endl;
}