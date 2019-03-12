//
// Created by hdc on 3/12/19.
//

// Singleton derived class cpp file

#include <iostream>
#include "derived.hpp"

sDerived::sDerived(int _initialValueX, int _initialValueY)
        : sBase(_initialValueX)
{
    mDataY =_initialValueY;
}

void sDerived::create(int _initialValueX, int _initialValueY)
{
    if(mpoSssInstance)
        std::cout << "Singleton has already been created" << std::endl;
    else
        mpoSssInstance = new sDerived(_initialValueX, _initialValueY);
}
