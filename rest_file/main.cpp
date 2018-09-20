#include <iostream>

using namespace std;
void simple();
int bigger(int a, int b);

int main() {
    std::cout << "Hello, World!" << std::endl;
    simple();
    bigger(5, 8);
    cout << bigger(7, 9) << endl;
    cin.get();
    return 0;
}

void simple()
{
    cout<< "I am simple\n";
}

int bigger(int a, int b)
{
    if(a>b)
        return a;
    else
        return b;
}