#include "TestA.h"

namespace MyInterface
{
	REGIST_BASE(DLLA)
}	
/// <summary>
/// ��������ʵ��
/// </summary>
/// <returns></returns>
MyInterface::IBase* GetBaseInterface()
{
	return new MyInterface::DLLA();
}