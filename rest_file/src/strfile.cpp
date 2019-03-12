//
// Created by hdc on 12/28/17.
//
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

int main()
{
    using namespace std;
    ifstream fin;
    fin.open("1.txt");
    if (fin.is_open() == false)
    {
        cerr << "cannot open file, bye.\n";
        exit(EXIT_FAILURE);
    }
    string item;
    int count = 0;

    getline(fin, item, ':');
    while (fin)
    {
        ++count;
        cout << count << ": " << item << endl;
        getline(fin, item, ':');
    }
    cout << "Done\n";
    fin.close();
    return 0;
}
