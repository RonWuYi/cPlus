//
// Created by HFD on 11/3/2017.
//
#include <iostream>
#include <string>
#include <w32api/synchapi.h>

using namespace std;
string version1(const string & s1, const string & s2);
const string & version2(string & s1, const string & s2);
const string & version3(string & s1, const string & s2);

int main(){
    string input;
    string copy;
    string result;

    cout << "Enter a string: ";
    getline(cin, input);
    copy = input;
    cout << "Your string is - " << input << endl;
    result = version1(input, "***");
    cout << "Your string enhanced - " << result << endl;
    cout << "Your original string - " << input << endl;

    result = version2(input, "###");
    cout << "Your string enhanced - " << result << endl;
    cout << "Your original string - " << input << endl;

    cout << "Resetting original string.\n";
    input = copy;
//    cout << "Your string is - " << input << endl;
    result = version3(input, "@@@");
    cout << "Your string enhanced - " << result << endl;
    cout << "Your original string - " << input << endl;

    cin.get();
    cin.get();
    cin.get();
    cin.get();
    Sleep(5000);
    return 0;
}

string version1(const string & s1, const string & s2)
{
    string temp;

    temp = s2 + s1 + s2;
    return temp;
}

const string & version2(string & s1, const string & s2)
{
    s1 = s2 + s1 + s2;
    return s1;
    Sleep(5000);
}

const string & version3(string & s1, const string & s2)
{
    string temp;

    temp = s2 + s1 + s2;
    return temp;
}

