#include <iostream>
#include <list>
#include <set>
#include <vector>
#include <string>
//using namespace std;

template<typename>
using void_t = void;

template<typename T, typename V = void>
struct has_push_back : std::false_type {};

template<typename T>
struct has_push_back<T, void_t<decltype(std::declval<T>().push_back(std::declval<typename T::value_type>()))>>:std::true_type{};


template<int N, int ...Ns>
int Sum()
{
	if constexpr(0 == sizeof...(Ns))
	{
		return N;
	}
	else
	{
		return N + Sum<Ns...>();
	}
}



struct ClassA
{
	int get_id()const
	{
		return 10;
	}
};

struct ClassB
{
	std::string get_name()const
	{
		return "hello";
	}
};


template<class T, class = void>
struct has_get_name : std::false_type {};

template<class T>
struct has_get_name<T,std::void_t<decltype(std::declval<T>().get_name())>> : std::true_type{};

// before c++17 
//template<typename T, typename std::enable_if<has_get_name<T>::value,int>::type = 0>
//std::string make_name(const T& cls)
//{
//	return cls.get_name();
//}
//
//template<typename T, typename std::enable_if<!has_get_name<T>::value, int>::type = 0>
//std::string make_name(const T& cls)
//{
//	return std::to_string(cls.get_id());
//}


//use c++17 if constexpr
template<typename T>
std::string make_name(const T& cls)
{
	if constexpr (has_get_name<T>::value)
	{
		return cls.get_name();
	}
	else
	{
		return std::to_string(cls.get_id());
	}
}


int main(int argc, char** argv)
{
	std::list<int> v;
	
	std::cout << has_push_back<std::set<int>>::value << std::endl;

	std::cout << Sum<1, 2, 3>() << std::endl;

	std::cout << has_get_name<ClassB>::value << std::endl;
	std::cout << has_get_name<ClassA>::value << std::endl;

	ClassA a;
	ClassB b;

	std::cout << make_name(a) << std::endl;
	std::cout << make_name(b) << std::endl;

	return 0;
}