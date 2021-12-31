/* 2021.12.31*/
#pragma	once
#include "Interface.h"
#include <map>
namespace MyInterface
{
	class DynamicLibrary;
#define DynamicLibPtr std::shared_ptr<DynamicLibrary>

	/// <summary>
	/// ��̬�������
	/// </summary>
	 class DLL_API DynLoader final : public ISingleton<DynLoader>
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
	protected:
		std::map<std::string, DynamicLibPtr >  m_dyns;
	};
}