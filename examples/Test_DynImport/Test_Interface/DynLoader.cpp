#include "DynLoader.h"
#include "DynamicLibrary.h"
namespace MyInterface
{
	/// <summary>
	/// ���ض�̬��
	/// </summary>
	/// <param name="path">��̬���ַ</param>
	/// <returns>��̬���ַ</returns>
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
	/// ж�ض�̬��
	/// </summary>
	/// <param name="lib">��̬��</param>
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
	/// ��ѯ��̬��
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	DynamicLibPtr DynLoader::findDynamicLibPtr(const std::string& name)
	{
		auto iter = m_dyns.find(name);
		return (iter != m_dyns.end()) ? iter->second : nullptr;
	}

}