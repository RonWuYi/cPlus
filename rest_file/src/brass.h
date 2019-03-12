//
// Created by HFD on 11/21/2017.
//

#ifndef BRASS_H_
#define BRASS_H_
class Brass
{
private:
    enum {MAX = 35};
    char fullname[MAX];
    long acctNum;
    double balance;
public:
    Brass(const char *s = "NONE", long an = -1, double bal = 0.0);
    virtual void ViewAcct()const ;
    virtual void Withdraw(double amt);
    void Deposit(double amt);
    double Balance()const ;
    virtual ~Brass(){}
};

class BrassPlus:public Brass{
private:
    double maxLoan;
    double rate;
    double owesBank;
public:
    BrassPlus(const char *s = "NONE", long an = -1, double bal = 0.0, double ml = 500, double r = 0.10);
    BrassPlus(const Brass &ba, double ml = 500, double r= 0.1);
    virtual void ViewAcct()const ;
    virtual void Withdraw(double amt);
    void ResetMax(double m){maxLoan = m;}
    void ResetRate(double r){rate = r;}
    void ResetOwes(){owesBank = 0;}
};
#endif //CPLUS_BRASS_H
