//
// Created by HFD on 11/17/2017.
//
#include <iostream>
#include <string>
#include <new>

using namespace std;
const int BUF = 512;

class JustTesting
{
private:
    string words;
    int number;
public:
    JustTesting(const string & s = "Just Testing", int n = 0){
        words = s;
        number = n;
        cout << words << " constructed\n";
    }
    ~JustTesting(){
        cout << words << " destroyed\n";
    }
    void Show()const {cout<<words<<", "<<number<<endl;}
};

    int main()
    {
        char * buffer = new char[BUF];
        JustTesting *pc1, *pc2;
        pc1 = new(buffer)JustTesting;
        pc2 = new JustTesting("Heap1", 20);

        cout << "Memory block addressed:\n" << "buffer: "
            <<(void*)buffer <<"    heap: " <<pc2 <<endl;
        cout << "Memory contents:\n";
        cout << pc1 <<": ";
        pc1->Show();

    }
