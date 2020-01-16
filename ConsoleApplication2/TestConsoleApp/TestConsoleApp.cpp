// TestConsoleApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

using namespace std;
int main()
{
    enum class Gender { Male, Female };

    enum class Gender2 { Male, Female };

    enum class Color { Red, Green, Blue };
 //   Gender gender = Gender::Male;
 ///*   Gender2 gender2 = Female;*/
 //   int Male = 10;
 //   cout << gender << endl;
 //       /*<< gender2;*/
 //   std::cout << "Hello World!\n";
    int Green = 10;
    Color x = Color::Green;
    if (x == Color::Red)
    {
        cout << "it's red\n";
    }
    else
    {
        cout << "it's not red\n";
    }

    cout << &x << endl;
    //cout << &(x+1) << endl;
    cout << int(x) << endl;

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
