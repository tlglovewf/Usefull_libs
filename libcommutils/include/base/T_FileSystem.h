/* 2022.01.12 by tuligen*/
#pragma once
#include <filesystem>
#include <algorithm>
#include <string>

#define _STR(x) #x
#define STR(x) _STR(x)
#define PRINTLOG(x) __pragma(message(__FILE__ "(" STR(__LINE__) ")"  ":" STR(x) ) )

#if __cplusplus > 201402L // 201703L)

namespace CommUtils
{

	/// <summary>
	/// �ַ����Ƚ�
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="caseinsensitive">��Сд������</param>
	/// <returns></returns>
	inline static bool stringcompare(const std::string& a, const std::string& b, bool caseinsensitive = false)
	{
		if (caseinsensitive)
		{
			return std::equal(a.begin(), a.end(),
				b.begin(), b.end(),
				[](char a, char b) {
					return tolower(a) == tolower(b);
				});
		}
		else
		{
			return a == b;
		}
	}

	/// <summary>
	/// ��Сдת��
	/// </summary>
	/// <param name="str">��ת���ִ�</param>
	/// <param name="tolower">true Сд/false ��д</param>
	inline static void transstringcase(std::string& str, bool tolower)
	{
		std::transform(str.begin(), str.end(), str.begin(), tolower ? ::tolower : ::toupper);
	}

	/// <summary>
	/// ��װc++17 �ļ�ϵͳ��
	/// </summary>
	class FileSystem final
	{
	public:
		/// <summary>
		/// ��̬���� ����Ҫ��������
		/// </summary>
		FileSystem() = delete;

		/// <summary>
		/// ����Ŀ¼
		/// </summary>
		/// <param name="path">·��</param>
		/// <returns>�����ɹ�</returns>
		static bool CreateDirectory(const std::string& path)
		{
			try
			{
				return std::filesystem::create_directories(path);
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
				return false;
			}
		}

		/// <summary>
		/// ɾ��
		/// </summary>
		/// <param name="path">·��</param>
		/// <returns>ɾ���ļ�����</returns>
		static int Delete(const std::string& path)
		{
			try
			{
				return static_cast<int>(std::filesystem::remove_all(path));
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
				return 0;
			}
		}

		/// <summary>
		/// ����	
		/// </summary>
		/// <param name="from">Դ·��</param>
		/// <param name="to">Ŀ��·��</param>
		static bool Copy(const std::string& from, const std::string& to)
		{
			try
			{
				std::filesystem::copy(from, to);
				return true;
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
				return false;
			}
		}

		/// <summary>
		/// �ļ��ƶ�
		/// </summary>
		/// <param name="from">Դ·��</param>
		/// <param name="to">Ŀ��·��</param>
		/// <returns></returns>
		static bool Move(const std::string& from, const std::string& to)
		{
			try
			{
				std::filesystem::rename(from, to);
				return true;
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
				return false;
			}
		}

		/// <summary>
		/// ������
		/// </summary>
		/// <param name="path">·��</param>
		/// <param name="newname">������</param>
		/// <returns></returns>
		static bool Rename(const std::string& path, const std::string& newname)
		{
			try
			{
				std::filesystem::path filepath(path);
				std::filesystem::path parentpath = std::move(filepath.parent_path());
				parentpath.append(newname);
				std::filesystem::rename(path, parentpath);
				return true;
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
				return false;
			}
		}

		/// <summary>
		/// �Ƿ����
		/// </summary>
		static inline bool IsExist(const std::string& path) { return std::filesystem::exists(path); }
	};


	/// <summary>
	/// Ŀ¼
	/// </summary>
	class Directory final
	{
	public:

		using StringList = std::vector<std::string>;

		/// <summary>
		/// ���캯��
		/// </summary>
		/// <param name="path">·��</param>
		Directory(const std::string& path) :_dir(path) {}


		/// <summary>
		/// �Ƿ����
		/// </summary>
		/// <returns></returns>
		inline bool exist()const
		{
			return _dir.exists();
		}

		/// <summary>
		/// �ж��Ƿ�ΪĿ¼
		/// </summary>
		/// <returns></returns>
		inline bool isDirectory()const
		{
			return _dir.is_directory();
		}

		/// <summary>
		/// �ж��Ƿ�Ϊ�ļ�
		/// </summary>
		/// <returns></returns>
		inline bool isFile()const
		{
			return _dir.is_regular_file() || _dir.is_block_file() || _dir.is_character_file();
		}

		/// <summary>
		/// ��ȡĿ¼�º�׺���ļ��б�
		/// </summary>
		/// <param name="extensions">��׺(.*)</param>
		/// <returns></returns>
		StringList getFiles(const StringList& extensions)
		{
			if (!isDirectory())
				return {};
			else
			{
				StringList filecontainer;
				std::filesystem::directory_iterator files(_dir);
				for (const auto& file : files)
				{
					if (!file.is_directory())
					{
						std::string ext = file.path().extension().string();
						//ת����Сд
						for (const auto& sfx : extensions)
						{
							if (stringcompare(sfx, ext, true))
								filecontainer.emplace_back(file.path().filename().string());
						}
					}
				}
				return filecontainer;
			}
		}

		/// <summary>
		/// ��ȡ��С
		/// </summary>
		/// <returns></returns>
		enum class szUnit//��λ
		{
			szBit,//�ֽ�
			szKb,
			szMb,
			szGb
		};
		double size(szUnit unit = szUnit::szBit)
		{

			if (exist())
			{
				if (_size == 0)
				{
					if (isDirectory())
					{
						_size = _calcsize(_dir);
					}
					else
					{

						_size = std::filesystem::file_size(_dir);

					}
				}

				switch (unit)
				{
				case szUnit::szBit:
					return static_cast<double>(_size);
				case szUnit::szKb:
					return double(_size) / 1024;
				case szUnit::szMb:
					return double(_size) / 1048576;		//1024 * 1024
				case szUnit::szGb:
					return double(_size) / 1073741824;	//1024 * 1024 * 1024
				default:
					return 0;
				}

			}
			else
			{
				return 0;
			}
		}

	protected:

		/// <summary>
		/// �����С
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		std::uintmax_t _calcsize(const std::filesystem::path& path)
		{
			std::filesystem::directory_entry entry(path);

			if (entry.is_directory())
			{
				std::uintmax_t size = 0;
				std::filesystem::directory_iterator files(entry);
				for (auto& file : files)
				{
					std::cout << "file: " << file.path().filename().string() << " " << file.file_size() << std::endl;
					size += _calcsize(file);
				}
				return size;
			}
			else
			{
				return entry.file_size();
			}
		}


	private:
		std::filesystem::directory_entry _dir;
		std::uintmax_t					 _size = 0;
	};

}
#else
#error "Please use c++17 or set /Zc:__cplusplus"
#endif 