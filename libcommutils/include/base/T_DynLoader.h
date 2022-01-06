/* 2021.12.31 by tuligen*/
#pragma	once
#include "T_Interface.h"
#include <map>
namespace CommUtils
{
	class DynamicLibrary;
#define DynamicLibPtr std::shared_ptr<DynamicLibrary>

	/// <summary>
	/// 动态库加载类
	/// </summary>
	 class COMM_API DynLoader final : public ISingleton<DynLoader>
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

		/// <summary>
		/// 设置插件地址
		/// </summary>
		/// <param name="path">路径</param>
		void setPluginPath(const std::string& path)
		{
			m_pluginpath = path;
		}

		/// <summary>
		/// 设置插件前缀
		/// </summary>
		/// <param name="prefix">前缀</param>
		void setPluginPrefix(const std::string& prefix)
		{
			m_pluginprefix = prefix;
		}

		/// <summary>
		/// 加载对象插件库
		/// </summary>
		/// <typeparam name="T">对象名</typeparam>
		/// <returns>是否加载成功</returns>
		bool loadTypePlugin(const std::string &name)
		{
			
#ifdef _WIN32
				std::string pluginabspath = m_pluginpath + m_pluginprefix + name + ".dll";
#else
				//add more platform
#endif
				auto ptr = DynLoader::getSingleton().load(pluginabspath);
				return ptr != nullptr;
			
		}
	protected:
		std::map<std::string, DynamicLibPtr >	m_dyns;
		std::string								m_pluginprefix	= "Plugin_";
		std::string								m_pluginpath	= "";
	};
}
#define LOADPLLUGIN(TYPE) CommUtils::DynLoader::getSingleton().loadTypePlugin(#TYPE)
