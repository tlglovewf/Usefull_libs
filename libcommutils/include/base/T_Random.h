#pragma once
#include <random>
#include <utility>

namespace CommUtils
{

	template<typename Engine = std::default_random_engine,
			 typename Distribution = std::uniform_int_distribution<> >
	/// <summary>
	/// ���������
	/// </summary>
	/// <typeparam name="Engine"></typeparam>
	/// <typeparam name="Distribution"></typeparam>
	class Random : public Distribution
	{
		using base_t = Distribution;
	public:
		using engine_type		= Engine;
		using result_type		= typename base_t::result_type;
		using param_type		= typename base_t::param_type;

	public:
	   /// <summary>
	   /// ���캯��
	   /// </summary>
	   /// <param name="...args">���������Χ(x~y)</param>
	   template<typename... T>
	   Random(T&&... args):
		   base_t(std::forward<T>(args)...),
		   _engine(std::random_device{}()){}
	   /// <summary>
	   /// ����һ�������
	   /// </summary>
	   /// <param name=""></param>
	   /// <returns></returns>
	   result_type operator()(void)
	   {
		   return base_t::operator()(_engine);
	   }

	   result_type operator()(const param_type& parm)
	   {
		   return base_t::operator()(_engine, parm);
	   }
	private:
		engine_type	_engine;
	};
}