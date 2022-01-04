/* 2022.01.04 by tuligen*/
#pragma once

#include "json/T_JsonInterface.h"
namespace CommUtils
{
	/// <summary>
	/// yyjson ������
	/// </summary>
	class Yyjson : public IJsonBase
	{
	public:
		/// <summary>
		/// ����json�ļ�
		/// </summary>
		/// <param name="path">json�ļ�·��</param>
		virtual void loadFile(const std::string& path) override;

		/// <summary>
		/// ����json����
		/// </summary>
		/// <param name="jsondata">json����</param>
		virtual void loadJson(const std::string& jsondata) override;
   };
}