//
// Created by ron on 10-12-17.
//
#include "studenti.h"
using namespace std;

// public methods
double Student::Average() const
{
    if (ArrayDb::size() > 0)
        return ArrayDb::sum()/ArrayDb::size();
    else
        return 0;
}

const string &Student::Name() const
{
    return (const string &) *this;
}

double& Student::operator[](int i)
{
    return ArrayDb::operator[](i);
}

double Student::operator[](int i) const
{
    return ArrayDb::operator[](i);
}

// private method
std::ostream& Student::arr_out(std::ostream &os) const
{
    int i;
    int lim = ArrayDb::size();
    if (lim > 0)
    {
        for (int i = 0; i < lim; i++)
        {
            os << ArrayDb::operator[](i) <<" ";
            if (i % 5 == 4)
            {
                os << endl;
            }
        }
        if (i % 5 != 0)
        {
            os << endl;
        }
    }
    else
    {
        os << " empty array ";
    }
    return os;
}

// friends
// use string version of operator>>()
istream &operator>>(istream &is, Student &stu)
{
    is >> (string &)stu;
    return is;
}

istream &getline(istream &is, Student &stu)
{
    getline(is, (string &)stu);
    return is;
}

ostream &operator<<(ostream &os, const Student &stu)
{
    os << "Scores for " << (const string &)stu << ":\n";
    stu.arr_out(os);
    return os;
}