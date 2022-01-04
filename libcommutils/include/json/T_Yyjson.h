/* 2022.01.04 by tuligen*/
#pragma once

#include "json/T_JsonInterface.h"
namespace CommUtils
{
	/// <summary>
	/// yyjson 处理类
	/// </summary>
	class Yyjson : public IJsonBase
	{
	public:
		/// <summary>
		/// 加载json文件
		/// </summary>
		/// <param name="path">json文件路径</param>
		virtual void loadFile(const std::string& path) override;

		/// <summary>
		/// 加载json内容
		/// </summary>
		/// <param name="jsondata">json数据</param>
		virtual void loadJson(const std::string& jsondata) override;
   };
}