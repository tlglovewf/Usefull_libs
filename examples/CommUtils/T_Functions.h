/* 2022.04.19 by tuligen*/
#pragma once

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
	};

	
}