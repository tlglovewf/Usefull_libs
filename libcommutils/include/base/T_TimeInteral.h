/* 2021.12.24 by tuligen*/
#pragma once
#include <chrono>
using namespace std;
namespace CommUtils
{
	/// <summary>
	/// ʱ������
	/// </summary>
	class TimeInteral final
	{
	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		TimeInteral()
		{
			//һ����ͼ�¼��ǰʱ��
			start();
		}
		//ɾ���������캯��
		TimeInteral(const TimeInteral&) = delete;

		/// <summary>
		/// ��¼��ǰʱ��
		/// </summary>
		void start()
		{
			_cur = std::chrono::system_clock::now();
		}

		/// <summary>
		/// ����ʱ����
		/// </summary>
		/// <param name="isreset">�Ƿ����õ�ǰʱ��</param>
		/// <returns>ʱ��s</returns>
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