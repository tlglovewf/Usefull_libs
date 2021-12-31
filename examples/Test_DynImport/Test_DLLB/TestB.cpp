#include "TestB.h"

namespace MyInterface
{
	REGIST_BASE(DLLB)
}
/// <summary>
/// 导出函数实现
/// </summary>
/// <returns></returns>
MyInterface::IBase* GetBaseInterface()
{
	return new MyInterface::DLLB();
}