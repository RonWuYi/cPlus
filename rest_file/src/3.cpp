#include <stddef.h>  // defines NULL
#include <iostream>
#include "base.hpp"

// Singleton base class cpp file

sBase* sBase::mpoSssInstance = 0;  // Global initialization to facilitate singleton design pattern

sBase::sBase(int _initialValueX)
        : mDataX(_initialValueX)
{
}

bool sBase::exists()
{
    return (mpoSssInstance != NULL); // Return true/false
}

sBase* sBase::instance()
{
    if(mpoSssInstance == 0) std::cout << "Class has not been created" << std::endl;

    return mpoSssInstance;
}
