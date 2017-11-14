//
// Created by HFD on 11/9/2017.
//

#ifndef UNTITLED_NAMESP_H
#define UNTITLED_NAMESP_H

#endif //UNTITLED_NAMESP_H
namespace pers{
    const int LEN = 40;
    struct Person
    {
        char fname[LEN];
        char lname[LEN];
    };
    void getPerson(Person &);
    void showPerson(const Person &);
}

namespace debts
{
    using namespace pers;
    struct Debt
    {
        Person name;
        double amount;
    };
    void getDebt(Debt &);
    void showDebt(const Debt &);
    double sumDebt(const Debt ar[], int n);
}
