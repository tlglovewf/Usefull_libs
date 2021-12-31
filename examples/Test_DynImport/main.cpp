#include <iostream>
#include "DynamicLibrary.h"
#include "DynLoader.h"
#include "Interface.h"


typedef MyInterface::IBase*  (*GetBase)();

int main(int argc, char** argv)
{
	auto apointer = MyInterface::DynLoader::getSingleton().load("Test_DLLA");
	auto bpointer = MyInterface::DynLoader::getSingleton().load("Test_DLLB");

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
	/***********�����ֽ���***********/
	/*ֱ��ʹ�ú�����ַ��ȡ����(����)*/
	/********************************/
	auto pt = static_cast<MyInterface::IBase* (*)()>(apointer->getProcAddress("GetBaseInterface"));
	(*pt)()->discript();

	pt = static_cast<MyInterface::IBase * (*)()>(bpointer->getProcAddress("GetBaseInterface"));
	(*pt)()->discript();
	getchar();
	return 0;
}