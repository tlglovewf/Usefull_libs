/* 2021.12.31 by tuligen*/
#pragma	once
#include "T_Interface.h"
#include <map>
namespace CommUtils
{
	class DynamicLibrary;
#define DynamicLibPtr std::shared_ptr<DynamicLibrary>

	/// <summary>
	/// ��̬�������
	/// </summary>
	 class COMM_API DynLoader final : public ISingleton<DynLoader>
	{
	public:
		/// <summary>
		/// Ĭ�Ϲ��캯��
		/// </summary>
		DynLoader() = default;
		/// <summary>
		/// ���ÿ���
		/// </summary>
		/// <param name=""></param>
		DynLoader(const DynLoader&) = delete;

		/// <summary>
		/// ���ض�̬��
		/// </summary>
		/// <param name="path">��̬���ַ</param>
		/// <returns>��̬���ַ</returns>
		DynamicLibPtr load(const std::string& path);
		
		/// <summary>
		/// ж�ض�̬��
		/// </summary>
		/// <param name="lib">��̬��</param>
		void unload(DynamicLibPtr lib);
		void unload(const std::string& name);

		/// <summary>
		/// ��ѯ��̬��
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		DynamicLibPtr findDynamicLibPtr(const std::string& name);

		/// <summary>
		/// ���ò����ַ
		/// </summary>
		/// <param name="path">·��</param>
		void setPluginPath(const std::string& path)
		{
			m_pluginpath = path;
		}

		/// <summary>
		/// ���ò��ǰ׺
		/// </summary>
		/// <param name="prefix">ǰ׺</param>
		void setPluginPrefix(const std::string& prefix)
		{
			m_pluginprefix = prefix;
		}

		/// <summary>
		/// ���ض�������
		/// </summary>
		/// <typeparam name="T">������</typeparam>
		/// <returns>�Ƿ���سɹ�</returns>
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
