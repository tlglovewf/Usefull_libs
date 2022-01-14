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
	/// 字符串比较
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="caseinsensitive">大小写不敏感</param>
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
	/// 大小写转换
	/// </summary>
	/// <param name="str">待转换字串</param>
	/// <param name="tolower">true 小写/false 大写</param>
	inline static void transstringcase(std::string& str, bool tolower)
	{
		std::transform(str.begin(), str.end(), str.begin(), tolower ? ::tolower : ::toupper);
	}

	/// <summary>
	/// 封装c++17 文件系统类
	/// </summary>
	class FileSystem final
	{
	public:
		/// <summary>
		/// 静态方法 不需要创建对象
		/// </summary>
		FileSystem() = delete;

		/// <summary>
		/// 创建目录
		/// </summary>
		/// <param name="path">路径</param>
		/// <returns>创建成功</returns>
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
		/// 删除
		/// </summary>
		/// <param name="path">路径</param>
		/// <returns>删除文件数量</returns>
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
		/// 拷贝	
		/// </summary>
		/// <param name="from">源路径</param>
		/// <param name="to">目标路径</param>
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
		/// 文件移动
		/// </summary>
		/// <param name="from">源路径</param>
		/// <param name="to">目标路径</param>
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
		/// 重命名
		/// </summary>
		/// <param name="path">路径</param>
		/// <param name="newname">新名称</param>
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
		/// 是否存在
		/// </summary>
		static inline bool IsExist(const std::string& path) { return std::filesystem::exists(path); }
	};


	/// <summary>
	/// 目录
	/// </summary>
	class Directory final
	{
	public:

		using StringList = std::vector<std::string>;

		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="path">路径</param>
		Directory(const std::string& path) :_dir(path) {}


		/// <summary>
		/// 是否存在
		/// </summary>
		/// <returns></returns>
		inline bool exist()const
		{
			return _dir.exists();
		}

		/// <summary>
		/// 判断是否为目录
		/// </summary>
		/// <returns></returns>
		inline bool isDirectory()const
		{
			return _dir.is_directory();
		}

		/// <summary>
		/// 判断是否为文件
		/// </summary>
		/// <returns></returns>
		inline bool isFile()const
		{
			return _dir.is_regular_file() || _dir.is_block_file() || _dir.is_character_file();
		}

		/// <summary>
		/// 获取目录下后缀名文件列表
		/// </summary>
		/// <param name="extensions">后缀(.*)</param>
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
						//转换到小写
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
		/// 获取大小
		/// </summary>
		/// <returns></returns>
		enum class szUnit//单位
		{
			szBit,//字节
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
		/// 计算大小
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