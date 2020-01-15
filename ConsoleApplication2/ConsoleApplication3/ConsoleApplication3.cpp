// ConsoleApplication3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

  // constructing threads
#include <iostream>       // std::cout
#include <atomic>         // std::atomic
#include <thread>         // std::thread
#include <vector>         // std::vector

std::atomic<int> global_counter(0);

void increase_global(int n) { for (int i = 0; i < n; ++i) ++global_counter; }

void increase_reference(std::atomic<int>& variable, int n) { for (int i = 0;   i < n; ++i) ++variable; }

struct C : std::atomic<int> {
    C() : std::atomic<int>(0) {}
    void increase_member(int n) { for (int i = 0; i < n; ++i) fetch_add(1); }
};

int main()
{
    std::vector<std::thread> threads;

    std::cout << "increase global counter with 10 threads...\n";
    for (int i = 1; i <= 100; ++i)
        threads.push_back(std::thread(increase_global, 90000000));

    std::cout << "increase counter (foo) with 10 threads using   reference...\n";
    std::atomic<int> foo(0);
    for (int i = 1; i <= 100; ++i)
    {
        threads.push_back(std::thread(increase_reference, std::ref(foo), 90000000));
    }

    std::cout << "increase counter (bar) with 10 threads using member...\n";
    C bar;
    for (int i = 1; i <= 100; ++i)
    {
        threads.push_back(std::thread(&C::increase_member, std::ref(bar), 90000000));
    }

    std::cout << "synchronizing all threads...\n";
    for (auto& th : threads) th.join();

    std::cout << "global_counter: " << global_counter << '\n';
    std::cout << "foo: " << foo << '\n';
    std::cout << "bar: " << bar << '\n';

    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
