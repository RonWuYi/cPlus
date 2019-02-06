#include <fstream>
#include <iterator>
#include <vector>
#include <iostream>


using namespace std;

int main(){
    std::ifstream input("/home/hdc/PycharmProjects/pyautogui/testfiles/PersistentStorage.bin", std::ios::binary);
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

    for (std::vector<unsigned char>::const_iterator i=buffer.begin(); i!= buffer.end() ; ++i) {
        std::cout << *i << ' ';
        return 0;
    }

//    ifstream file("", ios::in|ios::binary);
//
////    char ch;
////    char level[4];
////    char thetime[4];
//    char buff[128];
//    if (file.is_open()){
//        while (!file.eof()){
//            file.read(level);
////            file.getline(ch);
//        }
//    }



}