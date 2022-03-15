#include <iostream>
#include <tuple>
#include <set>
#include <map>
#include <variant>
#include <optional>
#include <string>
#include <memory>
#include <any>
#include <filesystem>

#include "test.h"
#include "T_FileSystem.h"
#include "T_Random.h"
using namespace std;


std::tuple<int, double> getfunc()
{
	return std::tuple(10, 22.2);
}


//结构化绑定
void test_strcutbind()
{
	std::pair test(10, 22.2);//自动推导 pair<int,double>

	auto [i, d] = getfunc();//结构化绑定 自动推导tuple里类型值
	std::cout << i << " " << d << std::endl;

	auto &[m, n] = test;
	n = 33.3;
	std::cout << m << " " << n << std::endl;

	std::map<int, std::string> mp =
	{
		{10,"one"},
		{11,"two"},
	};
	for (const auto &[x,y] : mp)
	{
		std::cout << x << " " << y << std::endl;
	}

	//还可以绑定数组和结构体
	int array[] = { 1,2,3 };
	auto [a, b, c] = array;
	std::cout << a << " " << b << " " << c << std::endl;
}

int getValue()
{
	int v;
	std::cout << "Compare with 10,Please input Numb: " << std::endl;
	cin >> v;
	return v;
}
//17 if-switch 表达式
void test_ifswitch()
{
	if (int t = getValue(); t < 10)
	{
		std::cout << "less" << std::endl;
	}
	else
	{
		std::cout << "great" << std::endl;
	}
}

//折叠
template<typename ... Ts >
auto Sum(Ts ... ts)
{
	return (ts + ...);
}

//constexpr 编译时期 lambda
//函数体不能包含汇编语句、goto语句、label、try块、静态变量、线程局部存储、
//没有初始化的普通变量，不能动态分配内存，不能有new delete等，不能虚函数
void test_constlambda()
{
	constexpr auto lamb = [](int n) {return n * n; };
	static_assert(lamb(3) == 9, "a");
}


namespace One::Two::Three
{
	struct Pt{};
}

//关键字
#if defined __has_include
#if !__has_include("test.h") // 判断是否有引用头文件
#error "test"
#endif
#endif

//如果 不使用返回值 报警告
[[nodiscard]] int dofunc()
{
	return 10;
}
void test_Attrib()
{
	int d;
	std::cout << "input value:" << std::endl;
	cin >> d;
	switch (d)
	{
	case 1:
		//add more
		std::cout << "get" << std::endl;
		[[fallthrough]]; //屏蔽警告？
	case 2:
		//[[fallthrough]]; //屏蔽警告？
	default:
		break;
	}
	dofunc();
	[[maybe_unused]] //消除未使用变量警告
	int tts;
}

//更高级的union
void test_variant()
{
	std::variant<int, std::string> test("test");
	std::cout << test.index() << std::endl;
	std::cout << std::get<std::string>(test) << std::endl;
	test = 10;
	std::cout << std::get<0>(test) << std::endl;

	std::variant<std::monostate,int, std::string> var;//模拟空状态
	var = 10;
}


std::optional<int> StoI(const std::string& str)
{
	try
	{
		return std::stoi(str);
	}
	catch (...)
	{
		return std::nullopt;
	}
}

void test_optional()
{
	std::string ss("d123");
	auto o = StoI(ss);
	if (o)
	{
		std::cout << *o << std::endl;
	}
	else
	{
		std::cout << "error numb." << std::endl;
	}
}

//any 存储任何单值
void test_any()
{
	std::any a = Test(10, 23);

	std::cout << a.type().name() << std::endl;

	auto b = std::any_cast<Test>(a);

	b.display();

}

//apply 可以直接解包
int add(int first, int second) { return first + second; }
void test_apply()
{
	std::cout << std::apply(add, std::pair(10, 32)) << std::endl;

	auto add_lambda = [](auto first, auto second) { return first + second; };

	std::cout << std::apply(add_lambda,std::tuple(33.2f,22.3f)) << std::endl;
}


void test_make_from_tuple()
{
	auto tup = std::make_tuple(32, 22);
	auto t = std::make_from_tuple<Test>(std::move(tup));
	t.display();
}

namespace fs = std::filesystem;

void test_filesystem()
{
	fs::path filepath("E:/CamCalibra/data/Ref/MATLAB_Calculate/Result/10.jpg");

	if (fs::exists(filepath))
	{
		std::cout << "file size : " << fs::file_size(filepath) << std::endl;  //字节
		std::cout << "exist." << std::endl;
		
		fs::directory_entry entry(filepath);
		auto status = entry.status();

		if (entry.is_directory())
		{
			
			fs::directory_iterator list(entry);
			for (auto& it : list)
			{
				std::cout << it.path().filename() << std::endl;
			}
		}
		else
		{
			std::cout << "it is a file" << std::endl;
			std::cout << entry.path().filename() << std::endl;
		}
	}
	else
	{
		std::cout << "not exist." << std::endl;
	}
}

enum Te
{
	eOne,
	eTwo
};

#include "T_Timer.h"



//void* operator new[](size_t size)
//{
//	cout << "new []" << endl;
//	return nullptr;
//}
void operator delete[](void *d)
{
	cout << "delete []" << endl;
}

void operator delete(void* d)
{
	cout << "delete" << endl;
}

template<typename ... Args>
std::string format(const std::string& pattern, Args ...args)
{
	size_t size = 1 + snprintf(nullptr, 0, pattern.c_str(), args ...);
	std::unique_ptr<char[]> buffer(new char[size]) ;
	snprintf(buffer.get(), size, pattern.c_str(), args ...);
	return std::string(buffer.get());
}

int main(int argc, char** argv)
{
	//test_strcutbind();
	//
	//test_ifswitch();
	//
	//std::cout << Sum(10, 12, 32) << std::endl;
	//
	//One::Two::Three::Pt pt;//命名空间嵌套
	//
	//
	//test_variant();
	//
	//test_optional();
	//
	//test_any();
	//
	//test_apply();
	//
	//test_make_from_tuple();

	//test_filesystem();

	//if (fs::remove("E:/tools/test.txt"))
	//{
	//	std::cout << "successfully." << std::endl;
	//}
	
	//int value = 1'000'000;
	//
	//std::cout << value << std::endl;

	auto str = format("%s%d", "你好", 100);

	std::cout << str.c_str() << std::endl;
	return 0;
}