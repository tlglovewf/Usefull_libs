#pragma once
#include <iostream>
#include "test_Interface.h"

namespace MyInterface
{
	class DLLB : public IBase
	{
	public:
		/*
		 * ����
		 */
		virtual void discript()const
		{
			std::cout << "Call Test B !!!" << std::endl;
		}
	};
}
PROCMARK  MyInterface::IBase* GetBaseInterface(); //���嵼������
