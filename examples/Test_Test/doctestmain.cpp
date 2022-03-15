#include <iostream>
//#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
//#include "doctest.h"
#include <functional>
using namespace std;

//bool func(int n)
//{
//	return n > 1;
//}
//
//TEST_CASE("hello world")
//{
//	REQUIRE(func(1)); //检查不通过 不再检查后续
//	CHECK(func(0));	  //检查不通过 记录继续后续
//	CHECK(func(6));
//}

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

int main(int argc, char** argv)
{

	//std::shared_ptr<TestA> a(new TestB);

	//cout << a.use_count() << endl;

	//std::shared_ptr<TestB> b = std::static_pointer_cast<TestB>(a);

	//cout << a.use_count() << endl;

	
	auto v = Factory<TestA>().create();

	auto f = Factory<TestB>().create(101);


	cout << typeid(v).name() << endl;
	cout << f->xx << endl;

	return 0;
}

#endif