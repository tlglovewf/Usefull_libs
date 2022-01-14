#include <iostream>
//启用mimalloc 直接引用头文件
#include <mimalloc-	new-delete.h>
#include <base\T_TimeInteral.h>
using namespace std;

int main(int argc, char **argv)
{

	CommUtils::TimeInteral timer;

	for (size_t i = 0; i < 1e8; ++i)
	{
		int* te = new int();
	}

	std::cout << timer.count() << std::endl;

	return 0;
}