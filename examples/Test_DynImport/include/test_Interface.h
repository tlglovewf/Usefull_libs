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
	/// ���Ի���
	/// </summary>
	class IBase
	{
	public:
		/*
		 * ���캯��
		 */
		IBase() = default;

		/*
		 * ��������
		 */
		virtual ~IBase() = default;

		/*
		 * ���ÿ���
		 */
		IBase(const IBase&) = delete;

		/*
		 * ����
		 */
		virtual void discript()const {};
	};

	//! ������ ��Ҫ�����Ҫ���̵߳Ķ�������
	class IBaseFactory
	{
	public:
		/*
		 * ���캯��
		 */
		virtual std::shared_ptr<IBase> create() = 0;
	};
	//class IBase  ע����
R_REGISTER(IBase)
#define REGIST_BASE(CLS)	REGIST_INSTANCE(CLS,	IBase)
#define UNREGIST_BASE(CLS)	UNREGIST_INSTANCE(CLS,	MyInterface::R_IBase)
#define GET_BASE(CLS)		GET_INSTANCE(CLS,		MyInterface::R_IBase)


/*
 * ����ע��(�ο�)
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