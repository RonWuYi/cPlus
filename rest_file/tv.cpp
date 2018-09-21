//
// Created by hdc on 12/27/17.
//
#include <iostream>
#include "tv.h"

bool Tv::volup()
{
    if (volume < MaxVal)
    {
        volume++;
        return true;
    } else
        return false;
}

bool Tv::voldown()
{
    if (volume > MinVal)
    {
        volume--;
        return true;
    } else
    {
        return false;
    }
}

void Tv::chanup()
{
    if (channel < maxchannel)
    {
        channel++;
    } else
    {
        channel = 0;
    }
}

void Tv::chandown() {
    if (channel > 0)
        channel--;
    else
        channel = maxchannel;
}

void Tv::settings() const
{
    using std::cout;
    using std::endl;
    cout << "TV is " << (state == Off?"Off":"On") <<endl;
    if(state == On)
    {
        cout << "Volume Setting = " << volume << endl;
        cout << "Channel Setting = " << channel << endl;
        cout << "Mode = "
             << (mode == Antenna?"antenna":"cable") << endl;
        cout << "Input = "
             << (input == TV?"TV":"VCR") << endl;
    }
}
