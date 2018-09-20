//
// Created by HFD on 11/9/2017.
//
#include <iostream>
#include <cstring>
#include "stock1.h"

Stock::Stock(){
    std::cout << "Default constructor called\n";
    std::strcpy(company, "no name");
    shares = 0;
    share_val = 0.0;
    total_val = 0.0;
    std::cout << std::endl;

}

Stock::Stock(const char * co, int n, double pr) {
    std::cout << "constructor using " << co << " called\n";
    std::strncpy(company, co, 29);
    company[29] = '\0';

    if (n < 0) {
        std::cerr << "number of shares can't be negative. "
                  << company << "share se to 0.\n";
        shares = 0;
    }
    else{
        shares = n;
    }
    share_val = pr;
    set_tot();

    std::cout << std::endl;
}
Stock::~Stock() {
    std::cout << "Bye, " << company << "!\n";
}


void Stock::buy(int num, double price) {
    if(num < 0)
    {
        std::cerr<<"number of shares purchased can't be negative. "
                 <<"transaction is aborted.\n";
    } else{
        shares+=num;
        share_val=price;
        set_tot();
    }
}
void Stock::sell(int num, double price) {
    using std::cerr;
    if (num < 0)
    {
        cerr<<"number of shares purchased can't be negative. "
                 <<"transaction is aborted.\n";
    } else if(num > shares)
    {
        cerr<<"You can't sell more than you have! "
            <<"transaction is aborted.\n";
    } else{
        shares -= num;
        share_val = price;
        set_tot();
    }
}

void Stock::update(double price) {
    share_val = price;
    set_tot();
}

void Stock::show() {
    using std::cout;
    using std::endl;

    cout<<"company: " << company
    <<"Shares: " << shares <<endl
    <<"Share price: $" << share_val
    <<"total worth: $" << total_val <<endl;
}

const Stock & topval(const Stock & s) const{
    if(s.total_val > total_val)
        return s;
    else
        return *this;
}