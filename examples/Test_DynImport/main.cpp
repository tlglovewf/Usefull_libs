#include <iostream>
#include "base\T_DynamicLibrary.h"
#include "base\T_DynLoader.h"
#include "test_Interface.h"


typedef MyInterface::IBase*  (*GetBase)();

int main(int argc, char** argv)
{
	auto apointer = CommUtils::DynLoader::getSingleton().load("Test_DLLA");
	auto bpointer = CommUtils::DynLoader::getSingleton().load("Test_DLLB");

	auto ibase = GET_BASE(DLLA);
	if (ibase)
	{
		ibase->discript();
	}
	ibase = GET_BASE(DLLB);
	if (ibase)
	{
		ibase->discript();
	}
	/***********华丽分界线***********/
	/*直接使用函数地址获取对象(反射)*/
	/********************************/
	auto pt = static_cast<MyInterface::IBase* (*)()>(apointer->getProcAddress("GetBaseInterface"));
	(*pt)()->discript();

	pt = static_cast<MyInterface::IBase * (*)()>(bpointer->getProcAddress("GetBaseInterface"));
	(*pt)()->discript();
	getchar();
	return 0;
}