#pragma once
#include <iostream>
#include "Interface.h"

namespace MyInterface
{
	class DLLA : public IBase
	{
	public:
		/*
		 * 描述
		 */
		virtual void discript()const
		{
			std::cout << "Call Test A !!!" << std::endl;
		}
	};


}
PROCMARK  MyInterface::IBase * GetBaseInterface(); //定义导出函数

