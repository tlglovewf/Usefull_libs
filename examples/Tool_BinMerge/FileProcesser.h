#pragma once
#include <vector>
#include <string>
#include <functional>
#include "PandarX32.h"
/// <summary>
/// 转换接口
/// </summary>
class ITrans
{
public:
	ITrans() = default;
	virtual ~ITrans() = default;	

	virtual std::string name()const = 0;

	virtual bool trans(const std::string& inputfile, const std::string& outfile) = 0;
};


/*
 * 文件处理类
 */
class FileProcesser	final
{
public:

	/// <summary>
	/// 合并文件(二进制字节拼接)
	/// </summary>
	/// <param name="files">文件集</param>
	/// <param name="output">输出文件</param>
	/// <returns>是否拼接成功</returns>
	static bool Binary_Merge(const std::vector<std::string>& files, const std::string& output, std::function<void (const std::string&)> func = nullptr);

	/// <summary>
	/// 合并文件(Pcap拼接)
	/// </summary>
	/// <param name="files"></param>
	/// <param name="output"></param>
	/// <param name="func"></param>
	/// <returns></returns>
	static bool Pcap_Merge(const std::vector<std::string>& files, const std::string& output, std::function<void(const std::string&)> func = nullptr);


	/// <summary>
	/// 文件差分(Pcap拆分)
	/// </summary>
	/// <param name="filepath">拆分路径</param>
	/// <param name="timelen">拆分时长</param>
	/// <returns></returns>
	static bool Pcap_Split(const std::string& filepath, int timelen = 20);

	/// <summary>
	/// 创建转换类
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	static std::shared_ptr<ITrans> CreateTrans(const std::string& name = "Pos");
protected:
	FileProcesser(const FileProcesser&) = delete;
};

class PcdTrans : public ITrans
{
public:
	virtual std::string name()const override { return "Pcd"; }

	virtual bool trans(const std::string& inputfile, const std::string& outfile) override;
};

class LasTrans : public ITrans
{
public:
	virtual std::string name()const override { return "Las"; }

	virtual bool trans(const std::string& inputfile, const std::string& outfile) override;
};

class PosTrans : public ITrans
{
public:
	virtual std::string name()const override { return "Pos"; }

	virtual bool trans(const std::string& inputfile, const std::string& outfile) override;
};


