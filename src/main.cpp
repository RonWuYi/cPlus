#include <iostream>
#include <string_view>
#include <thread>
#include <vector>

void parallel_parint(std::string_view s)
{
    std::vector<std::thread> threads;
    for (char c: s) threads.emplace_back([=](){
        std::cout << c;
    });
    for (auto& t: threads) t.join();
}

int main()
{
    parallel_parint("hello");
    return 0;
}