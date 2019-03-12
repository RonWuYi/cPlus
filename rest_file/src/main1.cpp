#include "derived.hpp"
#include <iostream>
using namespace std;

// Program which uses singleton class

main()
{
   // Only one instance of the singleton class can be created
   sDerived::create(3,3);

   sDerived::instance()->setDataX(5);
   cout << sDerived::instance()->getDataX() << endl;

   sDerived::instance()->setDataY(7);
   cout << sDerived::instance()->getDataY() << endl;
}
