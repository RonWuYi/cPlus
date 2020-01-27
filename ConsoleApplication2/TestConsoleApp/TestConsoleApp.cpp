#include <vector>
#include <iostream>

struct B {
	int m = 0;
	void hello() const {
		std::cout << "hello, this is B!\n";
	}
};

struct D : B
{
	void hello() const {
		std::cout << "hello, this is D!\n";
	}
};

enum class E { ONE = 1, TWO, THREE };
enum EU { ONE = 1, TWO, THREE };

int main()
{
	int n = static_cast<int>(3.14);
	std::cout << "n = " << n << '\n';

	std::vector<int> v = static_cast<std::vector<int>>(10);
	std::cout << "v.size() = " << v.size() << '\n';

	D d;
	B& br = d;
	br.hello();
	std::cout << "m of br is " << br.m << '\n';

	D& another_d = static_cast<D&>(br);
	another_d.hello();
	std::vector<int> v2 = static_cast<std::vector<int>&&>(v);

	std::cout << "check, v2.size() = " << v2.size() << '\n';
	std::cout << "after move, v.size() = " << v.size() << '\n';

	static_cast<void>(v2.size());

	void* nv = &n;
	int* ni = static_cast<int*>(nv);
	std::cout << "*ni = " << *ni << '\n';

	D a[10];
	B* dp = static_cast<B*>(a);
	a[1].hello();
	std::cout << a[1].m << std::endl;

	E e = E::ONE;
	int one = static_cast<int>(e);
	std::cout << "one is "<< one << '\n';

	E e2 = static_cast<E>(one);
	EU eu = static_cast<EU>(e2);

	//std::cout << "e2 is " << one << '\n';
	//std::cout << "eu is " << e2 << '\n';

	return 0;
}

