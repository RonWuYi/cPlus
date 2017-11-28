//
// Created by HFD on 11/28/2017.
//
#include "studentc.h"
using namespace std;

double Student::Average() const
{
    if (scores.size()>0)
    {
        return scores.sum()/scores.size();
    }
    else
    {
        return 0;
    }
}

const std::string& Student::Name() const
{
    return name;
}

double Student::operator[](int i) const
{
    return scores[i];
}

double &Student::operator[](int i)
{
    return scores[i];
}

std::ostream& Student::arr_out(std::ostream &os) const
{
    int i;
    int lim = scores.size();
    if (lim > 0)
    {
        for (int i = 0; i < lim; i++)
        {
            os << scores[i] << " ";
            if(i%5 == 4)
            {
                os<<endl;
            }
        }
    } else
    {
        os << " empty array ";
    }
    return os;
}

// friends
istream &operator>>(istream &is, Student &stu)
{
    is >> stu.name;
    return is;
}

istream &getline(istream &is, Student &stu)
{
    getline(is, stu.name);
    return is;
}

ostream &operator<<(ostream &os, const Student &stu)
{
    os << "Scores for " << stu.name << ":\n";
    stu.arr_out(os);
    return os;
}