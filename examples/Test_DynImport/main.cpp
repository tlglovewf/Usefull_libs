#include <iostream>
#include "DynamicLibrary.h"
#include "Interface.h"

int main(int argc, char** argv)
{

	auto dllpointer = MyInterface::DynamicLibrary::loadLibrary("Test_DLLA");
	dllpointer = MyInterface::DynamicLibrary::loadLibrary("Test_DLLB");

	auto ibase = GET_BASE(DLLB);
	if (ibase)
	{
		ibase->discript();
	}
	ibase = GET_BASE(DLLA);
	if (ibase)
	{
		ibase->discript();
	}
	getchar();
	return 0;
}