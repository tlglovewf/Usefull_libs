#include <iostream>
#include <fstream>

#include <base/T_Interface.h>
#include <base/T_DynLoader.h>
#include <base/T_JsonInterface.h>
using namespace std;


int main(int argc, char** argv)
{

	const char* jsonpath = "E:/Sources/QMTDataFactory/x64/Release/TypeL/rinexcfg.json";
	
#if 0
	if(LOADPLLUGIN(Yyjson))
	{

	//  auto item = GET_JSON(Yyjson);
	//  std::cout << "get " << item << std::endl;
	//  item->loadFile("test");
	}
#endif

	getchar();
	return 0;
}