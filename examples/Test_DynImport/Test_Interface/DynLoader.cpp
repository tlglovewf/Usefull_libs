#include "DynLoader.h"
#include "DynamicLibrary.h"
namespace MyInterface
{
	/// <summary>
	/// 加载动态库
	/// </summary>
	/// <param name="path">动态库地址</param>
	/// <returns>动态库地址</returns>
	DynamicLibPtr DynLoader::load(const std::string& path)
	{
		if (!path.empty())
		{
			DynamicLibPtr dynpt = findDynamicLibPtr(path);

			if (dynpt == nullptr)
			{
				dynpt = DynamicLibPtr(DynamicLibrary::loadLibrary(path));
			}
			if (dynpt != nullptr) m_dyns.insert({path, dynpt});
			return dynpt;

		}
		else
		{
			return nullptr;
		}
	}

	/// <summary>
	/// 卸载动态库
	/// </summary>
	/// <param name="lib">动态库</param>
	void DynLoader::unload(DynamicLibPtr lib)
	{
		if (lib != nullptr)
		{
			m_dyns.erase(lib->getName());
	   }
	}
	void DynLoader::unload(const std::string& name)
	{
		if (!name.empty())
		{
			m_dyns.erase(name);
		}
	}

	/// <summary>
	/// 查询动态库
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	DynamicLibPtr DynLoader::findDynamicLibPtr(const std::string& name)
	{
		auto iter = m_dyns.find(name);
		return (iter != m_dyns.end()) ? iter->second : nullptr;
	}

}