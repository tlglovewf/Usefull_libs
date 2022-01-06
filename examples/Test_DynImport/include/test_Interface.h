/* 2021.12.30*/
#pragma once
#include "base\T_Interface.h"

#include <string>
#include <memory>
#include <unordered_map>
#include <common_global.h>
namespace MyInterface
{

	/// <summary>
	/// 测试基类
	/// </summary>
	class IBase
	{
	public:
		/*
		 * 构造函数
		 */
		IBase() = default;

		/*
		 * 析构函数
		 */
		virtual ~IBase() = default;

		/*
		 * 禁用拷贝
		 */
		IBase(const IBase&) = delete;

		/*
		 * 描述
		 */
		virtual void discript()const {};
	};

	//class IBase  注册类
R_REGISTER(IBase)
#define REGIST_BASE(CLS)	REGIST_INSTANCE(CLS,	IBase)
#define UNREGIST_BASE(CLS)	UNREGIST_INSTANCE(CLS,	MyInterface::R_IBase)
#define GET_BASE(CLS)		GET_INSTANCE(CLS,		MyInterface::R_IBase)

#pragma endregion 
}