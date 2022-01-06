/* 2021.12.24 by tuligen*/
#pragma once
#include <chrono>
using namespace std;
namespace CommUtils
{
	/// <summary>
	/// 时间间隔类
	/// </summary>
	class TimeInteral final
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		TimeInteral()
		{
			//一构造就记录当前时间
			start();
		}
		//删除拷贝构造函数
		TimeInteral(const TimeInteral&) = delete;

		/// <summary>
		/// 记录当前时间
		/// </summary>
		void start()
		{
			_cur = std::chrono::system_clock::now();
		}

		/// <summary>
		/// 计算时间间隔
		/// </summary>
		/// <param name="isreset">是否重置当前时间</param>
		/// <returns>时间s</returns>
		float count(bool isreset = false)
		{
			auto cur = std::chrono::system_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(cur - _cur);
			if (isreset)
				_cur = std::move(cur);
			double rate = (double)std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;
			return duration.count() * rate;
		}
	protected:
		std::chrono::system_clock::time_point _cur;
	};
}