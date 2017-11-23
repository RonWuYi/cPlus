//
// Created by HFD on 11/23/2017.
//
#include <iostream>
#include <cstring>
#include "brass.h"

using namespace std;

Brass::Brass(const char *s, long an, double bal) {
    strncpy(fullname, s, MAX - 1);
    fullname[MAX - 1] = '\0';
    acctNum = an;
    balance = bal;
}

void Brass::Deposit(double amt) {
    if(amt < 0)
        cout << "no deposit, not allowed: "
             << "deposit is cancelled.\n";
    else
        balance += amt;
}

void Brass::Withdraw(double amt) {
    if(amt < 0)
        cout << "Withdraw should > 0: "
             << "Withdraw is cancelled.\n";
    else if(amt<=balance)
        balance -= amt;
    else
        cout<< "Withdrawal amout of $" << amt
           <<" exceeds your balance.\n"
           <<"Withdrawal canceled.\n";
}

double Brass::Balance() const {
    return balance;
}

void Brass::ViewAcct() const {
    ios_base::fmtflags initialState =
            cout.setf(ios_base::fixed, ios_base::floatfield);
    cout.setf(ios_base::showpoint);
    cout.precision(2);
    cout << "Client: " <<fullname << endl;
    cout << "Account Number: " <<acctNum << endl;
    cout << "Balance: $" <<balance << endl;
    cout.setf(initialState);
}

BrassPlus::BrassPlus(const char *s, long an, double bal, double ml, double r) :Brass(s, an, bal){
    maxLoan = ml;
    owesBank = 0.0;
    rate = r;
}

BrassPlus::BrassPlus(const Brass &ba, double m1, double r) :Brass(ba){
    maxLoan = m1;
    owesBank = 0.0;
    rate = r;
}