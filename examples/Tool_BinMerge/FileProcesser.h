#pragma once
#include <vector>
#include <string>
#include <functional>
#include "PandarX32.h"
/// <summary>
/// ת���ӿ�
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
 * �ļ�������
 */
class FileProcesser	final
{
public:

	/// <summary>
	/// �ϲ��ļ�(�������ֽ�ƴ��)
	/// </summary>
	/// <param name="files">�ļ���</param>
	/// <param name="output">����ļ�</param>
	/// <returns>�Ƿ�ƴ�ӳɹ�</returns>
	static bool Binary_Merge(const std::vector<std::string>& files, const std::string& output, std::function<void (const std::string&)> func = nullptr);

	/// <summary>
	/// �ϲ��ļ�(Pcapƴ��)
	/// </summary>
	/// <param name="files"></param>
	/// <param name="output"></param>
	/// <param name="func"></param>
	/// <returns></returns>
	static bool Pcap_Merge(const std::vector<std::string>& files, const std::string& output, std::function<void(const std::string&)> func = nullptr);


	/// <summary>
	/// �ļ����(Pcap���)
	/// </summary>
	/// <param name="filepath">���·��</param>
	/// <param name="timelen">���ʱ��</param>
	/// <returns></returns>
	static bool Pcap_Split(const std::string& filepath, int timelen = 20);

	/// <summary>
	/// ����ת����
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


