/* 2022.01.04 by tuligen*/
#pragma once
#include "base/T_Interface.h"

namespace CommUtils
{
	/// <summary>
	/// json 基础类
	/// </summary>
	class IJsonBase
	{
	public:
		IJsonBase() = default;
		virtual ~IJsonBase() = default;

		/// <summary>
		/// 加载json文件
		/// </summary>
		/// <param name="path">json文件路径</param>
		virtual void loadFile(const std::string& path) = 0;

		/// <summary>
		/// 加载json内容
		/// </summary>
		/// <param name="jsondata">json数据</param>
		virtual void loadJson(const std::string& jsondata) = 0;
	 };

//class IBase  注册类
R_REGISTER(IJsonBase)
#define REGIST_JSON(CLS)	REGIST_INSTANCE(CLS,	IJsonBase)
#define UNREGIST_JSON(CLS)	UNREGIST_INSTANCE(CLS,	R_IJsonBase)
#define GET_JSON(CLS)		GET_INSTANCE(CLS,		R_IJsonBase)
}

