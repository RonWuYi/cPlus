//
// Created by HFD on 12/11/2018.
//
#include <string.h>
#include <vector>
#include <iostream>
#include <cstdlib>

using namespace std;
bool accept(){
    cout << "do you want to processed\n";

    char answer = 0;
    cin >> answer;

    if(answer=='y' || answer=='yes'|| answer=='Y'|| answer=='Yes'|| answer=='YES')
        return true;

    return false;
}
class Point
{
public:
    Point()
    {
        cout<<"construction" <<endl;
    }
    Point(const Point& p)
    {
        cout<<"copy construction" <<endl;
    }
    ~Point()
    {
        cout<<"destruction" <<endl;
    }

};
int main()
{
//    vector<int>obj;
//    for (int i = 0; i < 10; ++i) {
//        obj.push_back(i);
//        cout<<obj[i]<<",";
//    }
    vector<Point> pointVec;
    Point a;
    Point b;
    pointVec.push_back(a);
    pointVec.push_back(b);

    cout<<pointVec.size()<<endl;
    return 0;
}
