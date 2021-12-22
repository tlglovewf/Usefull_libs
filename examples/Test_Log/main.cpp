#include <iostream>
#include <string>
#include <string_view>
using namespace std;


void* operator new (size_t count)
{
	std::cout << "alloc : " << count << endl;
	return malloc(count);
}

void operator delete(void *v)
{
	std::cout << "relloc ! " << endl;
	return free(v);
}

int main(int argc, char** argv)
{
	std::string str = "hello hddddddddddddddddddddddddddddddddd";
	std::string_view ss(str.c_str(), str.size());
	std::cout << ss.substr(10) << std::endl;
	std::cout << str.substr(10) << std::endl;

	std::cout << str << std::endl;

	std::cout << ss.data() << std::endl;

	getchar();
	return 0;
}