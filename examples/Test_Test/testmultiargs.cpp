#include <iostream>

#include <functional>
using namespace std;

template<typename T>
void print_v(T t)
{
	cout << "finish!" << endl;
}

template<typename V,typename... Args>
void print_v(V v, Args... args)
{
	cout << v << " ";
	print_v(args...);
}

class Te
{
public:

	void test(int x) { cout << x << " " << "test" << endl; }
};


auto getvalue()
{
	return std::tuple<int,int>{ 10,11 };
}

#if 1

#include <iostream>
#include <memory>
using namespace std;

class TestA
{
public:
	template<typename T, typename... Args>
	static std::shared_ptr<T> Create(Args ...args)
	{
		return std::make_pair<T>(args...);
	}
};

class TestB : public TestA
{
public:
	TestB() = default;
	TestB(int x):xx(x) {}

public:
	int xx;
};

class IFactory
{
public:
};

template<typename T>
class Factory
{
public:
	template<typename ... Arg>
	std::shared_ptr<T> create(Arg ... arg)
	{
		return std::make_shared<T>(arg...);
	}
};

template Factory<TestA>;
template Factory<TestB>;

int test_multiarg()
{
	auto v = Factory<TestA>().create();

	auto f = Factory<TestB>().create(101);


	cout << typeid(v).name() << endl;
	cout << f->xx << endl;

	return 0;
}

#endif