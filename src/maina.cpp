#include "header.h"
#include <iostream>

bool isAllowedToTakeFunRide()
{
    std::cout << "how tall\n";
    double height = double{};
    std::cin >> height;

    return (height - 140.0);
}

int main()
{
    if (isAllowedToTakeFunRide())
    {
        std::cout << "good";
    }
    else
    {
        std::cout << "bed";
    }
    return 0;
}