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

	//class IBase  ע����
R_REGISTER(IBase)
#define REGIST_BASE(CLS)	REGIST_INSTANCE(CLS,	IBase)
#define UNREGIST_BASE(CLS)	UNREGIST_INSTANCE(CLS,	MyInterface::R_IBase)
#define GET_BASE(CLS)		GET_INSTANCE(CLS,		MyInterface::R_IBase)

#pragma endregion 
}