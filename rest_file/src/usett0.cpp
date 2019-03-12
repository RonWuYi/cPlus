//
// Created by HFD on 11/21/2017.
//
#include <iostream>
#include "tabtenn0.h"

int main(void)
{
    using std::cout;
    TableTennisPlayer player1("ron", "wu", true);
    RatedPlayer player2(111, "next", "wu", false);
    player2.Name();
    if(player2.HasTable())
        cout<<": has a table.\n";
    else
        cout<<": does not have a table.\n";

    player1.Name();
    if(player1.HasTable())
        cout<<": has a table.\n";
    else
        cout<<": does not have a table.\n";
    cout << "Name: ";
    player2.Name();
    cout<<"; Rating: "<<player2.Rating()<<std::endl;

    RatedPlayer replayer2(1212, player1);
    cout << "Name: ";
    replayer2.Name();
    cout<<"; Rating: "<<replayer2.Rating()<<std::endl;


    std::cin.get();
    return 0;
}
