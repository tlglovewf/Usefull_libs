/* 2022.01.04 by tuligen*/
#pragma once
#include "base/T_Interface.h"

namespace CommUtils
{
	/// <summary>
	/// json ������
	/// </summary>
	class IJsonBase
	{
	public:
		IJsonBase() = default;
		virtual ~IJsonBase() = default;

		/// <summary>
		/// ����json�ļ�
		/// </summary>
		/// <param name="path">json�ļ�·��</param>
		virtual void loadFile(const std::string& path) = 0;

		/// <summary>
		/// ����json����
		/// </summary>
		/// <param name="jsondata">json����</param>
		virtual void loadJson(const std::string& jsondata) = 0;
	 };

//class IBase  ע����
R_REGISTER(IJsonBase)
#define REGIST_JSON(CLS)	REGIST_INSTANCE(CLS,	IJsonBase)
#define UNREGIST_JSON(CLS)	UNREGIST_INSTANCE(CLS,	R_IJsonBase)
#define GET_JSON(CLS)		GET_INSTANCE(CLS,		R_IJsonBase)
}

