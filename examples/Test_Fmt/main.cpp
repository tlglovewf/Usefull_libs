#include <iostream>
#include <fmt/fmt.h>
using namespace std;


int main(int argc, char** argv)
{

	std::cout << fmt::format("{0}-{1}-{2}", 10, 7, 9) << std::endl;;
	//auto s0 = std::format("{:6}", 42);    // s0 的值为 "    42"
	//auto s2 = std::format("{:*<6}", 'x'); // s2 的值为 "x*****"
	//auto s3 = std::format("{:*>6}", 'x'); // s3 的值为 "*****x"
	//auto s4 = std::format("{:*^6}", 'x'); // s4 的值为 "**x***"
	std::cout << fmt::format("{0:-<10}{1:-<10}{2:<10}{3:<10}{4:<10}", 10.123, 33.3333, 10, 244333, "hello") << std::endl;

	//print +,- synar
	std::cout << fmt::format("{0:},{0:-},{0:+}", 1) << std::endl;


	//进制转换	 0*b 0*d 0*x===> *x表示字符位数
	int x = 120;
	std::cout << fmt::format("{0:#08b},{0:-#08d},{0:#08x}",x) << std::endl;

	//小数点
	double y = 12345.123456789;
	std::cout << fmt::format("{0:.2f},{0:.10f}", y) << std::endl;
	std::cout << fmt::format("{:{}f}", y, 2) << std::endl;

	//unicode
	std::cout << fmt::format("{0},{1}", "你", "好") << std::endl;
	return 0;
}