#include <iostream>
#include <cstdlib>

int returnExample() 
{
    return EXIT_SUCCESS;
}

int returnFailure()
{
    return EXIT_FAILURE;
}

int getValue() { 
    std::cout << "enter an integer: ";
    int input{};
    std::cin >> input;
    return input;   
    }

void doPrint()
{
    std::cout << "In doPrint()\n";
}

int main()
{
    std::cout << "hello" << '\n';
    doPrint();
    std::cout << "enter main()\n";
    std::cout << returnExample() << '\n';
    std::cout << getValue() << '\n';

    std::cout << returnFailure() << '\n';
    return 0;
}