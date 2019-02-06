#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main(){
//    streampos size;
//    char* memblock;
//
//    //string line;
//    ifstream myfile ("/home/hdc/PycharmProjects/pyautogui/testfiles/PersistentStorage.bin", ios::in|ios::ate|ios::binary);
//    if (myfile.is_open()){
////        while (getline(myfile, line)){
////            cout<<line<<'\n';
////        }
////        myfile.close();
//        size = myfile.tellg();
//        memblock = new char[size];
//        myfile.seekg((0, ios::beg));
//        myfile.read(memblock, size);
//
//
//
//        myfile.close();
//        printf("%d\n", memblock);
//        cout<<"the entire file content is in memory";
//        for (unsigned int i = 0; i < sizeof(memblock); ++i) {
//            cout<<memblock[i];
//            cout<<memblock;
//        }
//        delete[] memblock;
//    } else
//        cout<<"unable to open\n";


    ifstream file("/home/hdc/PycharmProjects/pyautogui/testfiles/PersistentStorage.bin", ios::in|ios::ate|ios::binary);
    streamsize size = file.tellg();
    file.seekg(0, ios::beg);
    vector<uint8_t> buffer(size);
    if (file.read(buffer.data(), size)){
        for (unsigned i = 0; i < buffer.size(); i++) {
            cout<<' '<<buffer.at(i);
        }
        cout<<'\n';
    }
    return 0;
}


vector<BYTE> readFile(const char* filename){
    ifstream file(filename, ios::binary);
    file.unsetf(ios::skipws);

    streampos fileSize;
    file.seekg(0, ios::end);
    fileSize = file.tellg();
    file.seekg(0, ios::beg);
    vector<BYTE> vec;
    vec.reserve(fileSize);

}