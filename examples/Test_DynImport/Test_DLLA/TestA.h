#pragma once
#include <iostream>
#include "Interface.h"

namespace MyInterface
{
	class DLLA : public IBase
	{
	public:
		/*
		 * ����
		 */
		virtual void discript()const
		{
			std::cout << "Call Test A !!!" << std::endl;
		}
	};
}

