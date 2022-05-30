/* 2022.04.19 by tuligen*/
#pragma once
#include <string>
#include <fmt/fmt.h>
namespace CommUtils
{

	/// <summary>
	/// 常用函数
	/// </summary>
	class Functions final
	{
	public:
		/// <summary>
		/// 字节序转换
		/// </summary>
		/// <param name="t"></param>
		/// <returns></returns>
		template<typename T>
		static T SwitchByteStorage(T t)
		{
			constexpr size_t len = sizeof(T);
			char ch[len];
			char dst[len];
			memcpy(ch, &t, len);

			constexpr size_t half = len >> 1;
			for (size_t i = 0; i < half; ++i)
			{
				auto tempidx = len - 1 - i;
				ch[i] = ch[i] ^ ch[tempidx];
				ch[tempidx] = ch[i] ^ ch[tempidx];
				ch[i] = ch[i] ^ ch[tempidx];
			}

			T result;
			memcpy(&result, ch, len);
			return result;
		}

		/// <summary>
		/// 字符格式化输出
		/// </summary>
		/// <typeparam name="...Args">参数</typeparam>
		/// <typeparam name="N">格式化宽度</typeparam>
		/// <typeparam name="F">小数点位数</typeparam>
		/// <typeparam name="...Args">参数</typeparam>
		/// <returns></returns>
		template<int N,int F = 0, typename ... Args>
		static std::string WriteLine(Args ... args)
		{
			size_t sz = sizeof...(args);

			std::string _fmt;

			for (size_t i = 0; i < sz; ++i)
			{
				std::string temp;
				if (F != 0)
				{
					temp = "{" + std::to_string(i) + fmt::format(":<{0}.{1}f", N, F) + "}";
				}
				else
				{
					temp = "{" + std::to_string(i) + fmt::format(":<{:}", N) + "}";
				}
				_fmt += temp;
			}
			_fmt += "\n";
			return fmt::format(_fmt, args...);
		}

		/// <summary>
		/// 周秒转天秒
		/// </summary>
		/// <param name="wktime"></param>
		/// <returns></returns>
		static inline double WkTime2DayTime(double wktime)
		{
			return fmod(wktime, 86400);
		}
#if __cplusplus > 201402
		/// <summary>
		/// 切分字符串
		/// </summary>
		/// <param name="strv"></param>
		/// <param name="delims"></param>
		/// <returns></returns>
		static std::vector<std::string_view> splitSV(std::string_view strv, std::string_view delims = " ")
		{
			std::vector<std::string_view> output;
			size_t first = 0;

			while (first < strv.size())
			{
				const auto second = strv.find_first_of(delims, first);

				if (first != second)
					output.emplace_back(strv.substr(first, second - first));

				if (second == std::string_view::npos)
					break;

				first = second + 1;
			}

			return output;
		}
#endif

		/// <summary>
		/// 将vector插入到另一个vector中
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="rt"></param>
		/// <param name="ins"></param>
		/// <param name="index"></param>
		template<typename T>
		static void VecInsert(std::vector<T>& rt, const std::vector<T>& ins, int index)
		{
			index = std::min(std::max(index, 0), (int)rt.size());
			size_t orilen = rt.size();
			rt.resize(orilen + ins.size());
			if (index != orilen)
			{
				std::vector<T> temp(&rt[index], &rt[orilen]);

				memcpy(&rt[index + ins.size()], &temp[0], sizeof(T) * temp.size());
			}
			memcpy(&rt[index], &ins[0], sizeof(T) * ins.size());
		}
	};

	
}