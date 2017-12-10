//
// Created by ron on 10-12-17.
//
#include <iostream>
#include "studenti.h"
using namespace std;

void set(Student &sa, int n);

const int pupils = 3;
const int quizzes = 5;

int main()
{
    Student ada[pupils] =
            {Student(quizzes), Student(quizzes), Student(quizzes)};

    int i;
    for (int i = 0; i < pupils; ++i)
    {
        set(ada[i], quizzes);
    }
    cout << "\nStudent List:\n";
    for (int i = 0; i < pupils; ++i)
    {
        cout << ada[i].Name() << endl;
    }
    cout << "\nResults:";
    for (int i = 0; i < pupils; ++i)
    {
        cout << endl << ada[i];
        cout << "average: " << ada[i].Average() << endl;
    }
    cout << "Done. \n";
    return 0;
}

void set(Student &sa, int n)
{
    cout << "Please enter the student's name: ";
    getline(cin, sa);

}