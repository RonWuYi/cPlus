#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(){
    string line;
    ifstream myfile ("/home/hdc/PycharmProjects/pyautogui/testfiles/PersistentStorage.bin", ios::out|ios::app|ios::binary);
    if (myfile.is_open()){
        while (getline(myfile, line)){
            cout<<line<<'\n';
        }
        myfile.close();
    } else
        cout<<"unable to open\n";
    return 0;
}