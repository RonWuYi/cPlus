#include <iostream>
#include <fstream>
#include <bitset>
//#include <vector>
#include <math.h>
#include <iomanip>
#include <stdio.h>
#include <string.h>
using namespace std;

int main(){
    streampos size;
    char* memblock;
    char d[2000];
    //string line;
    ifstream myfile ("/home/hdc/PycharmProjects/pyautogui/testfiles/PersistentStorage.bin", ios::in|ios::ate|ios::binary);
    if (myfile.is_open()){
//        while (getline(myfile, line)){
//            cout<<line<<'\n';
//        }
//        myfile.close();
        size = myfile.tellg();
        memblock = new char[size];
        myfile.seekg((0, ios::beg));
        myfile.read(memblock, size);
        myfile.close();
//        printf("%d\n", memblock);
        cout<<"the entire file content is in memory\n";
//        for (unsigned int i = 0; i < sizeof(memblock); ++i) {
//            cout<<memblock[i];
//            cout<<memblock
//        }

//        cout<<memblock;
//        for (unsigned int i = 0; i < size; ++i) {
//            if (i >2){
//                bitset<16> x(memblock[i]);
//    //            cout<<bitset<8>[i]<<endl;
//                cout<<setbase(16);
//                cout << x;}
//        }

//        long int longint = 0;
//        string buf;
//        for (int i = 0; i < size; ++i) {
//            longint+=(memblock[size-i-1]-48)*pow(2, i);
//        }

//
//        size_t got = cin.gcount();
//        char* out = buffer;
//
//        for (auto j = buffer; j < buffer+got; j+=4) {
//            *out++ = Perfect_Hash::
//        }
        memcpy(d, memblock, strlen(memblock));
                for (unsigned int i = 0; i < size; ++i) {
            if (i >2){
               // bitset<16> x(memblock[i]);
    //            cout<<bitset<8>[i]<<endl;
//                cout<<setbase(16);
                cout << std::hex<<(int)d[i];}
        }
        delete[] memblock;
    } else
        cout<<"unable to open\n";
    return 0;
}