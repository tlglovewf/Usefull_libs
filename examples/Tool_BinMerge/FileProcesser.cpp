#include "FileProcesser.h"
#include "T_Functions.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <streambuf>
#include <regex>

#include <pcap.h>

/// <summary>
/// 合并文件(二进制拼接)
/// </summary>
/// <param name="files">文件集</param>
/// <param name="output">输出文件</param>
/// <returns>是否拼接成功</returns>
bool FileProcesser::Binary_Merge(const std::vector<std::string>& files, const std::string& output, std::function<void(const std::string&)> func/* = nullptr*/)
{
	using namespace std::filesystem;
	if (files.size() < 2 || !exists(files[0]))
		return false;

	std::ifstream pre(files[0], std::ios::binary);

	std::ofstream outfile(output,std::ios::binary);
	
	std::string content;
	content.assign(std::istreambuf_iterator<char>(pre), std::istreambuf_iterator<char>());

	if (func)
	{
		func(files[0]);
	}
	outfile.write(content.c_str(), content.size());

	for (size_t i = 1; i < files.size(); ++i)
	{
		if (func)
		{
			func(files[i]);
		}
		if (!exists(files[i]))
		{
			continue;
		}	
		std::ifstream cur(files[i], std::ios::binary);
		content.assign(std::istreambuf_iterator<char>(cur), std::istreambuf_iterator<char>());
		outfile.write(content.c_str(), content.size());
	}


	return true;
}

/// <summary>
/// 创建转换类
/// </summary>
/// <param name="name"></param>
/// <returns></returns>
std::shared_ptr<ITrans> FileProcesser::CreateTrans(const std::string& name /*= "Pos"*/)
{
	if (name == "Las")
	{
		return std::make_shared<LasTrans>();
	}
	else if (name == "Pcd")
	{
		return std::make_shared<PcdTrans>();
	}
	else if( name == "Pos")
	{
		return std::make_shared<PosTrans>();
	}
	//add more types ...
	else
	{
		return nullptr;
	}
}

extern bool ReadPcapFile(_Pcapfile& file);

struct pktStruct {
	struct pcap_pkthdr pkt_header; // header object
	const u_char* pkt_data; // data object
	long time; // used to compare with each other
};
/**
 * Method Name: comparison()
 * Arguments  : pktStruct, pktStruct
 * Return Type: bool
 * Description: callback method for sort() method
 */
bool comparison(pktStruct a, pktStruct b) {
	return a.time < b.time;
}
/**
 * Method Name: sortPcap(对pcap文件中时间进行排序,防止出现乱序的文件情况)   
 * Arguments  : pcap_t *
 * Return Type: void
 * Description: sort a .pcap file by time stamp then write all the packets to a new .pcap file
 */
void sortPcap(pcap_t* spcap, const std::string sortedFile) {

	struct pcap_pkthdr* header; // header object
	const u_char* data; // data object which is a pointer

	const u_char* data_copy; // data_copy

	std::vector<pktStruct> pktVector; // this vector contains each pktStruct
	pktStruct myStruct; // every myStruct instance stores a packet

	while (int returnValue = pcap_next_ex(spcap, &header, &data) >= 0) {

		// store each packet
		myStruct.pkt_header = *header;
		data_copy = (u_char*)malloc(myStruct.pkt_header.caplen); // malloc space for data_copy
		memcpy((void*)data_copy, (const void*)data, // copy data to data_copy
			myStruct.pkt_header.caplen);
		myStruct.pkt_data = data_copy;
		myStruct.time = header->ts.tv_sec * 1000000 + header->ts.tv_usec; // time is used to compare with each other
		/* print for test */
//		for (u_int i = 0; i < header->caplen; i++) {
		//	Start printing on the next after every 16 octets
//			if ((i % 16) == 0)
//				cout << endl;
//			//	Print each octet as hex (x), make sure there is always two characters (.2).
//			printf("%.2x ", myStruct.pkt_data[i]);
//			myStruct.pkt_data[i]=data[i];
//		}
//		/* another way to value the struct */
//		myStruct = {
//			header,
//			data,
//			time
//
//		};
		pktVector.push_back(myStruct);

		/* print for test */
//        cout<< "packetNo."<<":"<<structCount<<endl;
//		for (u_int i = 0; (i < header->caplen); i++) {
////				 Start printing on the next after every 16 octets
//			if ((i % 16) == 0)
//				cout << endl;
////				 Print each octet as hex (x), make sure there is always two characters (.2).
//			printf("%.2x ", myStruct.pkt_data[i]);
//
//		}
//		cout << myStruct.time << endl; // print for test
//		cout << myStruct.pkt_header->caplen << endl; //print for test
//		cout << myStruct.pkt_header->len << endl; // print for test
	}
	pcap_close(spcap);
	std::cout << "done store" << std::endl;
	/* reorder the vector by time */
	sort(pktVector.begin(), pktVector.end(), comparison);
	std::cout << "done sort" << std::endl;

	/* dump the ordered packets to the new file */
	pcap_t* finalPcap = pcap_open_dead(DLT_EN10MB, 262144); // dumper will use it
	pcap_dumper_t* tmpDump = pcap_dump_open(finalPcap,sortedFile.c_str()); // will be used to dump packets to the new file

	for (std::vector<pktStruct>::size_type ix = 0; ix < pktVector.size(); ix++) {
		pcap_dump((unsigned char*)tmpDump, &pktVector.at(ix).pkt_header,
			pktVector.at(ix).pkt_data);
		std::cout << ix << ":" << pktVector.at(ix).time << std::endl; // print for test

//		for (u_int i = 0; (i < header->caplen); i++) {
//			//				 Start printing on the next after every 16 octets
//			if ((i % 16) == 0)
//				cout << endl;
//			//				 Print each octet as hex (x), make sure there is always two characters (.2).
//			printf("%.2x ", pktVector.at(ix).pkt_data[i]);

		std::cout << pktVector.at(ix).pkt_header.caplen << std::endl; //print for test
		std::cout << pktVector.at(ix).pkt_header.len << std::endl; // print for test

	}

	pcap_dump_close(tmpDump);
	std::cout << "done dump" << std::endl; //print for test
	pcap_close(finalPcap);
	return;
}

/// <summary>
/// 合并文件(Pcap拼接)
/// </summary>
/// <param name="files"></param>
/// <param name="output"></param>
/// <param name="func"></param>
/// <returns></returns>
bool FileProcesser::Pcap_Merge(const std::vector<std::string>& files, const std::string& output, std::function<void(const std::string&)> func /*= nullptr*/)
{
	using namespace std::filesystem;
	if (files.size() < 2 || !exists(files[0]))
		return false;

	char errbuff[PCAP_ERRBUF_SIZE] = { 0 };
	pcap_t* prepcap = pcap_open_offline(files[0].c_str(), errbuff);
	pcap_pkthdr* header = nullptr;

	const UCHAR* data;

	pcap_t* presult = pcap_open_dead(DLT_EN10MB, 262144);
	pcap_dumper_t* dumper = pcap_dump_open(presult, output.c_str());
	
	
	if (func)
	{
		func(files[0]);
	}

	pcap_loop(prepcap, 0, pcap_dump, (UCHAR*)dumper);
	pcap_close(prepcap);
	for (size_t i = 1; i < files.size(); ++i)
	{
		if (func)
		{
			func(files[i]);
		}
		if (!exists(files[i]))
		{
			continue;
		}

		pcap_t* pcurcap = pcap_open_offline(files[i].c_str(), errbuff);
		pcap_loop(pcurcap, 0, pcap_dump, (UCHAR*)dumper);
		pcap_close(pcurcap);
	}
	
	pcap_dump_close(dumper);


	return true;
}

bool PcdTrans::trans(const std::string& inputfile, const std::string& outfile)
{
	return true;
}

bool LasTrans::trans(const std::string& inputfile, const std::string& outfile)
{
	if (inputfile.empty() || outfile.empty())
	{



		return false;
	}
	else
	{

	}

	return true;
}

bool PosTrans::trans(const std::string& inputfile, const std::string& outfile)
{
	if (inputfile.empty() || outfile.empty())
	{
		
		return false;
	}
	else
	{
		_Pcapfile pcapfile;
		pcapfile._path = inputfile;
		if (ReadPcapFile(pcapfile))
		{
			std::ofstream  ofile(outfile);

			for (const auto& item : pcapfile._pos)
			{
				ofile.precision(10);
				ofile << item.lat << "," << item.lon << "," << item.gpstime << ",0,0,0" << std::endl;
			}
		}
		else
		{
			return false;
		}
	}

	return true;
}

#pragma comment(lib,"wpcap.lib")
static bool ReadPcapFile(_Pcapfile& file)
{
	if (std::filesystem::exists(file._path))
	{
		pcap_t* fp = nullptr;
		char errbuf[PCAP_ERRBUF_SIZE] = { 0 };
		/* Open the capture file */
		if ((fp = pcap_open_offline(file._path.c_str(),			// name of the device
			errbuf					// error buffer
		)) == NULL)
		{
			file._broken = true;
			return false;
		}


		_DataPacket _dp;
		uint64_t timestamp = 0;
		int res = 0;
		struct pcap_pkthdr* header = nullptr;
		const u_char* pkt_data     = nullptr;

		/* Retrieve the packets from the file */
		while ((res = pcap_next_ex(fp, &header, &pkt_data)) >= 0)
		{
			/* print pkt timestamp and pkt len */
			//1122 表示点云数据包字节数
			if (header->len == 1122)
			{
				//复制内存块
				memcpy((char*)&_dp, pkt_data, sizeof(_DataPacket));

				
			}
			//554 表示gps数据包字节数字
			if (header->len == 554)
			{
				char _szgprmc[100] = { 0 };
				std::copy(pkt_data + 60, pkt_data + 60 + 100, _szgprmc);

				std::string _str(_szgprmc);
				std::string _gprmc = "";
				static const std::regex rgx("^\\$GPRMC,[\\d\\.]*,[A|V],(-?[0-9]*\\.?[0-9]+),([NS]*),(-?[0-9]*\\.?[0-9]+),([EW]*),.*");

				auto _slist = CommUtils::Functions::splitSV(_str, ",");
				file._gpscnt++;
				if (!std::regex_match(_str, rgx) || (_slist.size() != 13))
				{
					std::cout << _str << " GPRMC DATA ERROR!" << std::endl;
					continue;
				}
				PosItem item;
				auto wsec = std::atof(_slist[1].data());
				auto lng  = std::atof(_slist[5].data()) * 0.01;
				auto lat  = std::atof(_slist[3].data()) * 0.01;
				item.lon = (int)lng + (lng - (int)lng) * 5 / 3.0;
				item.lat = (int)lat + (lat - (int)lat) * 5 / 3.0;
				
				//UIT 2 GPS 要加闰秒
				item.gpstime = (int)wsec % 100 + ((int)(wsec / 100)) % 100 * 60 + ((int)(wsec / 10000)) * 3600 + 18;
				file._pos.emplace_back(item);
			}
		}
		//包损坏了
		if (res == -1)
		{
			file._broken = true;
			pcap_close(fp);
			return false;
		}
		else
		{
			pcap_close(fp);
			return true;
		}
	}
	else
	{
		return false;
	}
}

#include <sstream>
static std::string addtime(const std::string& info, int t = 20)
{
	int h, m, s;//时分秒
	sscanf(info.c_str(), "%2d%2d%2d", &h, &m, &s);

	size_t total = h * 3600 + m * 60 + s;

	total += t;

	h = total / 3600;
	m = (total - h * 3600) / 60;
	s = total % 60;

	std::stringstream ss;
#define FMT std::setfill('0') << std::setw(2)
	ss << FMT << h << FMT << m << FMT << s;
#undef FMT

	return ss.str();
}

#include <QDebug>
/// <summary>
/// 文件差分(Pcap拆分)
/// </summary>
/// <param name="filepath">拆分路径</param>
/// <param name="timelen">拆分时长</param>
/// <returns></returns>
bool FileProcesser::Pcap_Split(const std::string& filepath, int timelen /*= 20*/)
{
	int timecount = 0;

	if (std::filesystem::exists(filepath))
	{
		pcap_t* fp = nullptr;
		char errbuf[PCAP_ERRBUF_SIZE] = { 0 };
		size_t nps = filepath.find_last_of('/') + 1;
		std::string folderpath = filepath.substr(0, nps);
		std::string filename   = filepath.substr(nps);

		size_t eps = filename.find_first_of('-');
		std::string prjname = filename.substr(0,eps);
		std::string suffix = filename.substr(eps);

		int h, m, s;//时分秒
		std::string timestr = prjname.substr(prjname.size() - 6);
		prjname = prjname.substr(0, prjname.size() - 6);
		sscanf(timestr.c_str(), "%2d%2d%2d", &h, &m, &s);

		const std::string tempname = filepath + "_temp";

		//std::filesystem::copy(filepath, tempname);
		
		timestr = addtime(timestr);
		/* Open the capture file */
		if ((fp = pcap_open_offline((tempname).c_str(),		// name of the device
									errbuf					// error buffer
			)) == NULL)
		{
			return false;
		}
		else
		{
			int res = 0;
			struct pcap_pkthdr* header = nullptr;
			const u_char* pkt_data = nullptr;

			pcap_dumper_t* p_dumper = nullptr;
			
			/* Retrieve the packets from the file */
			while ((res = pcap_next_ex(fp, &header, &pkt_data)) >= 0)
			{
				if (p_dumper == nullptr)
				{
					std::string outpath = folderpath + prjname + timestr + suffix;

					p_dumper = pcap_dump_open(fp, outpath.c_str());
					if (p_dumper == nullptr)
					{
						std::cout << "create pcap error: " << errbuf << std::endl;
					}
					timestr = addtime(timestr);
				}
				pcap_dump((u_char*)p_dumper, header, pkt_data);
				
				//554 表示gps数据包字节数字
				if (header->len == 554)
				{
					++timecount;
				}
				// 以20s 切分一个包
				if (timecount == 20)
				{
					pcap_dump_close(p_dumper);
					p_dumper = nullptr;
					timecount = 0;
				}
			}
			//生成最后一段
			if (timecount > 0)
			{
				pcap_dump_close(p_dumper);
			}
		}

		pcap_close(fp);
		//std::filesystem::remove(tempname);
	}

	return true;
}