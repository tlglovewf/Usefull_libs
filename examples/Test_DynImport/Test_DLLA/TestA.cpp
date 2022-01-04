#include "TestA.h"

namespace MyInterface
{
	REGIST_BASE(DLLA)
}	
/// <summary>
/// 导出函数实现
/// </summary>
/// <returns></returns>
MyInterface::IBase* GetBaseInterface()
{
	return new MyInterface::DLLA();
}