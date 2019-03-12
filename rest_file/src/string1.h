//
// Created by Ron on 11/15/2017.
//
#include <iostream>
using std::ostream;
using std::istream;

#ifndef STRING1_H_
#define STRING1_H_
class String
{
private:
    char * str;
    int len;
    static int num_strings;
    static const int CINLIM= 80;
public:
    explicit String(const char * s);
    String();
    String(const String &);
    ~String();
    int length()const {return len;}
    // overloaded operator
    String &operator= (const String &);
    String &operator= (const char *);
    char &operator[](int i);
    const char & operator[](int i) const;
    friend bool operator<(const String &st, const String &st2);
    friend bool operator>(const String &st1, const String &st2);
    friend bool operator==(const String &st, const String &st2);
    friend ostream & operator<<(ostream &os, const String &st);
    friend istream & operator>>(istream &is, String &st);

    // static function
    static int HowMany();
};
#endif //CPLUS_STRING1_H
