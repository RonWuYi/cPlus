//
// Created by HFD on 11/9/2017.
//
#include <iostream>
#include "namesp.h"

namespace pers
{
    using std::cout;
    using std::cin;
    void getPersion(Person & rp){
        cout<<"enter first name: ";
        cin >> rp.fname;
        cout<<"enter last name: ";
        cin >> rp.lname;
    }

    void showPersion(const Person & rp)
    {
        std::cout << rp.lname << ", " << rp.fname;
    }
}

namespace debts{
    void getDebt(Debt & rd){
        getPerson(rd.name);
    }
}
