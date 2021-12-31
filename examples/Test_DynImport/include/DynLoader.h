/* 2021.12.31*/
#pragma	once
#include "Interface.h"
#include <map>
namespace MyInterface
{
	class DynamicLibrary;
#define DynamicLibPtr std::shared_ptr<DynamicLibrary>

	/// <summary>
	/// 动态库加载类
	/// </summary>
	 class DLL_API DynLoader final : public ISingleton<DynLoader>
	{
	public:
		/// <summary>
		/// 默认构造函数
		/// </summary>
		DynLoader() = default;
		/// <summary>
		/// 禁用拷贝
		/// </summary>
		/// <param name=""></param>
		DynLoader(const DynLoader&) = delete;

		/// <summary>
		/// 加载动态库
		/// </summary>
		/// <param name="path">动态库地址</param>
		/// <returns>动态库地址</returns>
		DynamicLibPtr load(const std::string& path);
		
		/// <summary>
		/// 卸载动态库
		/// </summary>
		/// <param name="lib">动态库</param>
		void unload(DynamicLibPtr lib);
		void unload(const std::string& name);

		/// <summary>
		/// 查询动态库
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		DynamicLibPtr findDynamicLibPtr(const std::string& name);
	protected:
		std::map<std::string, DynamicLibPtr >  m_dyns;
	};
}