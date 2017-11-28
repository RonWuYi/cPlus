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
    virtual void Withdraw(double amt) = 0; // pure virtual function
    double Balance()const { return balance;};
    virtual void ViewAcct()const = 0;
    virtual ~AcctABC(){}
};

class Brass: public AcctABC
{
public:
    Brass(const char *s = "Nullbody", long an = -1,
          double bal = 0.0): AcctABC(s, an , bal){}
    virtual void Withdraw(double amt);
    virtual void ViewAcct()const;
    virtual ~AcctABC(){};
};

class BrassPlus: public AcctABC
{
private:
    double maxLoan;
    double rate;
    double owesBank;
public:
    BrassPlus(const char *s = "Nullbody", long an = -1,
              double bal = 0.0, double ml = 500, double r =  0.10);
    BrassPlus(const Brass & ba, double ml = 500, double r = 0.1);
    virtual void Withdraw(double amt);
    virtual void ViewAcct()const;
    void ResetMax(double m){maxLoan = m;}
    void ResetRate(double r){rate = r;}
    void ResetOwes(){owesBank = 0;}
};
#endif //CPLUS_ACCTABC_H
