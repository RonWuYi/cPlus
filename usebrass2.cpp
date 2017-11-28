//
// Created by Ron on 11/24/2017.
//
#include <iostream>
#include "acctabc.h"
//const int CLIENTS = 4;
const int CLIENTS = 2;
const int LEN = 40;

int main()
{
    using namespace std;
    AcctABC * p_clients[CLIENTS];

    int i;
    for (i = 0; i < CLIENTS; i++) {
        char temp[LEN];
        long tempnum;
        double tempbal;
        char kind;
        cout<<"Enter client's name: ";
        cin.getline(temp, LEN);
        cout<<"Enter client's account number: ";
        cin>>tempnum;
        cout<<"Enter balance: ";
        cin>>tempbal;
        cout<<"enter 1 for Brass or"
            <<"2 for BrassPlus Account: ";
        while (cin>>kind&&(kind!='1' && kind!='2'))
            cout<<"Enter either 1 or 2: ";

        if (kind=='1')
            p_clients[i] = new Brass(temp, tempnum, tempbal);
        else
        {
            double tmax, trate;
            cout<<"Enter the overdraft limit: $";
            cin>>tmax;
            cout<<"Enter the rate "
                <<"as a decimal fraction: ";
            cin>>trate;
            p_clients[i] = new BrassPlus(temp, tempnum, tempbal, tmax, trate);
        }
        while (cin.get()!='\n')
            continue;
        cout<<endl;
        for (i = 0; i < CLIENTS; i++) {
            p_clients[i]->ViewAcct();
            cout<<endl;
        }

        for (i = 0; i < CLIENTS; i++) {
            delete p_clients[i];
        }
        cout<<"Done.\n";
        cin.get();
        cin.get();
        cin.get();
        cin.get();
        return 0;
    }
}
