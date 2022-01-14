#include <iostream>
#include <string>
#include <string_view>

#include <base/T_Interface.h>
#include <base/T_DynLoader.h>
#include <base/T_JsonInterface.h>
using namespace std;


int main(int argc, char** argv)
{
	int x;

	if (LOADPLLUGIN(Yyjson))
	{
		
		auto item = GET_JSON(Yyjson);
		std::cout << "get " << item << std::endl;
		item->loadFile("test");
	}
	std::cout << "over" << std::endl;
	getchar();
	return 0;
}