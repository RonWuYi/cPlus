#include <any>
#include <iostream>
#include <cstdlib>
#include <string_view>
#include <memory>
#include <vector>
#include <fstream>
#include <cassert>
#include <functional>

int add(int x, int y);
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

void printValues(int x, int y)
{
    std::cout << x << '\n';
    std::cout << y << '\n';
}

int getValueFromUser()
{
    std::cout << "enther an integer: ";
    int input{};
    std::cin >> input;

    return input;
}

void printDouble(int value)
{
    std::cout << value << " double is: " << value * 2 << '\n';
}

class BasePacket {
public:
    virtual ~BasePacket() = default;

    virtual bool Generator() = 0;
    virtual bool Verify(std::string_view) { return true; }
};

// class NetworkPacket : public BasePacket {
// public:
//     NetworkPacket() = default;
//     bool Generator() { return true; }
//     bool Verify(std::string_view config) const override { 
//         std::cout << "verify against: " << config;
//         return true; 
//     }
// private:
//     std::any data_;
// };
struct B {
    virtual void bar() { std::cout << "B bar\n"; }
    virtual ~B() = default;
};

struct D : B
{
    D() {std::cout << "D::D\n";}
    ~D() {std::cout << "D::~D\n";}
    void bar() override { std::cout << "D bar\n"; }
};

std::unique_ptr<D> pass_through(std::unique_ptr<D> p)
{
    p->bar();
    return p;
}

void close_file(std::FILE* fp) { std::fclose(fp); }

struct List {
    struct Node {
        int data;
        std::unique_ptr<Node> next;
        Node(int data) : data{data}, next{nullptr} {}
    };
    List() : head{nullptr} {};
    ~List() { while(head) head = std::move(head->next); }

    void push(int data) {
        auto temp = std::make_unique<Node>(data);
        if (head) 
        {
            temp->next = std::move(head);
        }
        
        head = std::move(temp);
    }
private:
    std::unique_ptr<Node> head;
};

int main()
{
    std::cout << "demo\n";
    {
        auto p = std::make_unique<D>();
        auto q = pass_through(std::move(p));
        assert(!p);
        q->bar();
    }

    std::cout << "Runtime polymorphism demo\n";

    {
        std::unique_ptr<B> p = std::make_unique<D>();
        p->bar();

        std::vector<std::unique_ptr<B>> v;
        v.push_back(std::make_unique<D>());
        v.push_back(std::move(p));
        v.emplace_back(new D);
        for (auto& p : v)
        {
            p->bar();
        }
    }

    std::cout << "custom delete demo\n";
    std::ofstream("demo.txt") << 'x';
    {
        std::unique_ptr<std::FILE, decltype(&close_file)> fp(std::fopen("demo.txt", "r"),
                                                            &close_file);
                                                            
        if (fp)
        {
            std::cout << (char)std::fgetc(fp.get()) << '\n';
        }
        
    }

    // std::cout << "custom lambda-expression delete demo\n";
    // {
    //     std::unique_ptr<D, std::function<void(D*)>> p(new D, [](D* ptr)
    //     {})
    // }
    // return 0;
    std::cout << add(4, 50) << '\n';
    return 0;

}

int add(int x, int y)
{
    return x + y;
}