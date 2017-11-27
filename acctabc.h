//
// Created by HFD on 11/27/2017.
//

#ifndef CPLUS_ACCTABC_H
#define CPLUS_ACCTABC_H

#include <bits/ios_base.h>

class AcctABC
{
private:
    enum {MAX = 35};
    char fullName[MAX];
    long acctNum;
    double balance;
protected:
    const char *FullName()const {
        return fullName;
    }
    long AcctNum()const { return acctNum;}
    std::ios_base::fmtflags SetFormat()const ;
public:
    AcctABC(const char *s = "Nullbody", long an = -1,
            double bal = 0.0);
    void Deposit(double amt);
};
#endif //CPLUS_ACCTABC_H
