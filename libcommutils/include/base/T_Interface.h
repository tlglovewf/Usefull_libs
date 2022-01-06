/* 2021.12.30 by tuligen*/
#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <comm_global.h>
namespace CommUtils
{
	/// <summary>
	/// 单例基类
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	class ISingleton
	{
	public:
		ISingleton() = default;
		virtual ~ISingleton() = default;

		static T& getSingleton()
		{
			static T instance;
			return instance;
		}
	};

#pragma region  插件注册类

	/// <summary>
	/// 插件基础类
	/// </summary>
	/// <typeparam name="T">基类</typeparam>
	template<typename T>
	class IPluginBase
	{
	public:
		IPluginBase(const std::string& name, std::shared_ptr<T> ptr) :_name(name), _ptr(ptr) {}
		/*
		 * 获取名称
		 */
		std::string name()const { return _name; }
		/*
		 * 获取内容
		 */
		std::shared_ptr<T> content()const { return _ptr; }
	protected:
		std::string				_name;
		std::shared_ptr<T>      _ptr;
	};

#define PluginPtr(T)  std::shared_ptr<IPluginBase<T> > 

	/// <summary>
	/// 注册机
	/// </summary>
	/// <typeparam name="T">注册基类</typeparam>
	template<typename T>
	class Register 
	{
	public:
		Register<T>() = default;
		virtual ~Register<T>() = default;
		Register<T>(const Register&) = delete;

		void regist(PluginPtr(T) ptr)
		{
			if (ptr != nullptr)
			{
				mItems.emplace(std::make_pair(ptr->name(), ptr->content()));
			}
		}

		void unregist(PluginPtr(T) ptr)
		{
			if (ptr != nullptr)
			{
				mItems.erase(ptr->name());
			}
		}

		void unregist(const std::string& name)
		{
			if (!name.empty())
			{
				mItems.erase(name);
			}
		}

		std::shared_ptr<T> getObject(const std::string& name)
		{
			auto iter = mItems.find(name);
			return (iter == mItems.end()) ? nullptr : iter->second;
		}
	protected:
		using ItemContainer = std::unordered_map<std::string, std::shared_ptr<T> >;
		ItemContainer mItems;
	};


	/// <summary>
	/// 插件实例类
	/// </summary>
	/// <typeparam name="T">插件实体类</typeparam>
	/// <typeparam name="C">实体类基类</typeparam>
	/// <typeparam name="R">基类注册类</typeparam>
	template<typename T, typename C, typename R>
	class PluginInstance
	{
	public:
		PluginInstance(const std::string& name)
		{
			static auto t = std::make_shared<IPluginBase<C> >(name, std::make_shared<T>());
			R::getSingleton().regist(t);
		}
	protected:
		std::shared_ptr<T> _ptr;
	};

#define PLUGININSTANCE(CLS,D)  CommUtils::PluginInstance<CLS,D, R_##D>

//单例实现体
#define SINGLEIMPL(N)   R_##N&  R_##N::getSingleton(){ \
							static R_##N single;  \
							return single;	  \
						}

/*
 * 特化注册类
 */
#define DEFSINGLE(N)  public:\
						static	N& getSingleton();

#define R_REGISTER(N)   \
	class COMM_API R_##N : public CommUtils::Register<N>	\
	{											\
		DEFSINGLE(R_##N)						\
	};

 //实例注册类
#define REGIST_INSTANCE(CLS,C) \
			 PLUGININSTANCE(CLS,C) s_##CLS(#CLS);
#define UNREGIST_INSTANCE(CLS,R) \
			MyInterface::R::getSingleton().unregist(#CLS);
#define GET_INSTANCE(CLS,R)		\
			MyInterface::R::getSingleton().getObject(#CLS);
}
