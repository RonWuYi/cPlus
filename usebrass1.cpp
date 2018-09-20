//
// Created by Ron on 11/24/2017.
//
#include <iostream>
#include "brass.h"

int main()
{
    using namespace std;

    Brass Piggy("Porcelot Pigg", 381299, 4000.00);
    BrassPlus Hoggy("Horatio Hogg", 382288, 3000.00);
    Piggy.ViewAcct();
    cout<<endl;
    Hoggy.ViewAcct();
    cout<<endl;
    cout<<"Deopsiting $1000 into Hogg Account:\n";
    Hoggy.Deposit(1000.00);
    cout<<"New balance: $" << Hoggy.Balance()<<endl;
    cout<<"Withdrawing $4200 from the Pigg Account:\n";
    Piggy.Withdraw(4200.00);
    cout<<"Piggy's new balance: $" << Piggy.Balance()<<endl;
    cout<<"Withdrawing $4200 from the Hoggy Account:\n";
    Hoggy.Withdraw(4200.00);
    Hoggy.ViewAcct();
    cin.get();
    return 0;
}
