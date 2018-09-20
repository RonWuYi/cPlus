//
// Created by hdc on 12/11/17.
//

#ifndef CPLUS_WORK0_H
#define CPLUS_WORK0_H

#include <string>

class Worker
{
private:
    std::string fullname;
    long id;

protected:
    virtual void Data() const ;
    virtual void Get();
public:
    Worker(): fullname ("no one"), id(0L){}
    Worker(const std::string &s, long n)
            : fullname(s), id(n){}
    virtual ~Worker()=0;
    virtual void Set();
    virtual void Show()const ;
};

class Waiter: virtual public Worker
{
private:
    int panache;

protected:
    void Data() const ;
    void Get();
public:
    Waiter():Worker(), panache(0){}
    Waiter(const std::string &s, long n, int p = 0)
            : Worker(s, n), panache(p){}
    Waiter(const Worker &wk, int p = 0)
            :Worker(wk), panache(p){}
    void Set();
    void Show()const ;
};


class Singer: virtual public Worker
{
protected:
    enum {other, alto, contralto, soprano, bass, baritione, tenor};
    enum {Vtypes = 7};
    void Data()const ;
    void Get();

private:
    static char *pv[Vtypes];
    int voice;
public:
    Singer(): Worker(), voice(other){}
    Singer(const std::string &s, long n, int v = other)
            :Worker(s, n), voice(v){}
    Singer(const Worker &wk, int v = other)
            :Worker(wk), voice(v){}
    void Set();
    void Show() const ;
};

class SingingWaiter: public Singer, public Waiter
{
protected:
    void Data()const ;
    void Get();
public:
    SingingWaiter(){};
    SingingWaiter(const std::string &s, long n, int p = 0,
                    int v = other)
            :Worker(s, n), Waiter(s, n, p), Singer(s, n, v){}

    SingingWaiter(const Worker &wk, int p = 0, int v = other)
            :Worker(wt), Waiter(wk, p), Singer(wk, v){}

    SingingWaiter(const Waiter &wt, int v = other)
            :Worker(wt), Waiter(wt), Singer(wt, v){}

    SingingWaiter(const Singer &wt, int p)
            :Worker(wt), Waiter(wt, p), Singer(wt){}
    void Set();
    void Show()const ;
};
#endif //CPLUS_WORK0_H
