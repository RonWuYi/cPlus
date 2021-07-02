#include <array>
#include <sstream>
#include <locale>
#include <iostream>
#include <typeinfo>
#include <iterator>
#include <thread>
#include <future>
#include <vector>
#include <memory>
#include <mutex>
#include <queue>
#include <chrono>
#include <string>
#include <cstdlib>
#include <ctime>

class Timer
{
private:
	// Type aliases to make accessing nested type easier
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;
	
	std::chrono::time_point<clock_t> m_beg;
 
public:
	Timer() : m_beg(clock_t::now())
	{
	}
	
	void reset()
	{
		m_beg = clock_t::now();
	}
	
	double elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}
};

struct myData {
    int value;
};

struct Song
{
    Song(std::string s1, std::string s2)
    {
        name = s1;
        title = s2;
    }
    std::string name;
    std::string title;
};

std::unique_ptr<Song> SongFactory(const std::string& artist, const std::string& title)
{
    return std::make_unique<Song>(artist, title);
}

// struct
std::mutex myDataMutex;

void pushAndPrint(std::vector<std::unique_ptr<myData>> &vu1, int &seconds)
{
    for (int i = 0; i < 50000; ++i)
    {

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::this_thread::sleep_for(std::chrono::milliseconds(seconds));
        std::cout << "sleep for push and pushAndPrint function more " << seconds << " seconds" << std::endl;
        std::srand(static_cast<unsigned int>(std::time(nullptr))); // set initial seed value to system clock
        auto u = std::make_unique<myData>();

        u->value = rand();
        // for (int i = 0; i < 5; ++i)
        // {
            
        // }
        myDataMutex.lock();
        vu1.push_back(std::move(u));
        myDataMutex.unlock();
    }
}

std::mutex sizeMutex;
void printSize(std::vector<std::unique_ptr<myData>> &vu1, int &seconds)
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(seconds));
        sizeMutex.lock();
        std::cout << "seconds from " << &seconds << std::endl;
        std::cout << "sleep_for " << seconds << " seconds" << std::endl;
        std::cout << "vector size is " << vu1.size() << std::endl;
        if (vu1.size() > 20)
        {
            seconds += 1;
        }
        sizeMutex.unlock();
    }
}

std::mutex popMutex;
void popV(std::vector<std::unique_ptr<myData>> &vu1)
{
    while (true)
    {
        if (vu1.size() > 5)
        {
            /* code */
            popMutex.lock();
            std::cout << vu1.front()->value << std::endl;
            vu1.erase(vu1.begin());
            popMutex.unlock();
        }
        else {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        
    }

}


int main()
{
    auto p1 = std::make_shared<myData>();
    auto p2 = std::make_unique<myData>();

    std::vector<std::unique_ptr<myData>> myData1;

    unsigned int n = std::thread::hardware_concurrency();
    std::cout << n << " concurrent threads are supported.\n";
    int seconds = 5;
    std::thread t1(pushAndPrint, std::ref(myData1), std::ref(seconds));
    std::thread t2(printSize, std::ref(myData1), std::ref(seconds));
    std::thread t3(popV, std::ref(myData1));
    t2.join();
    t1.join();
    t3.join();

    Timer t;
    std::cout << "hello" << std::endl;
    std::cout << t.elapsed() << std::endl;
    return 0;
}
