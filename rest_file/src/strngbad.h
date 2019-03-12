//
// Created by Ron on 11/14/2017.
//
#include <iostream>
#ifndef STRNGBAD_H_
#define STRNGBAD_H_
class StringBad
{
private:
    char * str;
    int len;
    static int num_strings;
public:
    StringBad(const char * s);
    StringBad();
    ~StringBad();
    // friend function
    friend std::ostream &operator<<(std::ostream & os, const StringBad & st);
};
#endif //CPLUS_STRNGBAD_H
