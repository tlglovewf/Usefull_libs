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

	//! 工厂类 主要解决需要多线程的对象问题
	class IBaseFactory
	{
	public:
		/*
		 * 构造函数
		 */
		virtual std::shared_ptr<IBase> create() = 0;
	};
	//class IBase  注册类
R_REGISTER(IBase)
#define REGIST_BASE(CLS)	REGIST_INSTANCE(CLS,	IBase)
#define UNREGIST_BASE(CLS)	UNREGIST_INSTANCE(CLS,	MyInterface::R_IBase)
#define GET_BASE(CLS)		GET_INSTANCE(CLS,		MyInterface::R_IBase)


/*
 * 工厂注册(参考)
 */
#define REGIST_DBASE(CLS) \
			class CLS##Factory : public IBaseFactory\
			{\
			public:	\
				virtual std::shared_ptr<IBase> create()\
				{ \
					return std::make_shared<CLS>();\
				}\
			};\
		REGIST_INSTANCE(CLS##Factory,IBase);

#define CREATE_BASE(CLS) \
		GET_BASE(CLS)->create();
#pragma endregion 
}