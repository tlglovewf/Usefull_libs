#include "TestB.h"

namespace MyInterface
{
	REGIST_BASE(DLLB)
}
/// <summary>
/// ��������ʵ��
/// </summary>
/// <returns></returns>
MyInterface::IBase* GetBaseInterface()
{
	return new MyInterface::DLLB();
}