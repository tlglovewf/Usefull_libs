#pragma once
#include <memory>

#pragma pack(push)
#pragma pack(1)
//扫描仪单点数据结构
typedef struct tag_DataPoint
{
	unsigned short distance;
	unsigned char calref;
	char reserved;			//XT32 预留1字节
public:
	tag_DataPoint() {
		memset(this, 0, sizeof(tag_DataPoint));
	}
}_DataPoint;

//扫描仪单面数据结构
typedef struct tag_DataBlock
{
	//unsigned short flag;
	unsigned short azimuth;
	//单点结构
	_DataPoint datapoints[32];
public:
	tag_DataBlock() {
		//flag = (short)0xFFEE;
		memset(this, 0, sizeof(tag_DataBlock));
	}
}_DataBlock;

//XT32 包头
typedef struct tag_PacketHeader {
	char packetHeader[6];
public:
	tag_PacketHeader() {
		memset(this, 0, sizeof(tag_PacketHeader));
	}
}_PacketHeader;

//XT32 数据头
typedef struct tag_DataHeader {
	char dataHeader[6];
public:
	tag_DataHeader() {
		memset(this, 0, sizeof(tag_DataHeader));
	}
}_DataHeader;

//XT32 数据尾部
typedef struct tag_DataRear {
	char reserved[13];
	unsigned char year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	uint32_t timestamp;
	char factoryInfo;
public:
	tag_DataRear() {
		memset(this, 0, sizeof(tag_DataRear));
	}
}_DataRear;

//附加信息
typedef struct tag_AppendInfo {

	//UDP Sequence 该数据包的序列号，从1 到0xFF FF FF FF，小端节序
	unsigned int udpSeq;
public:
	tag_AppendInfo() {
		memset(this, 0, sizeof(tag_AppendInfo));
	}
}_AppendInfo;

//扫描仪单个数据包数据结构
typedef struct tag_DataPacket
{
	//数据头
	char header[42];

	//包头
	_PacketHeader packetHeadr;

	//数据头
	_DataHeader dataHeader;

	//单面结构
	_DataBlock datablocks[8];

	//数据尾
	_DataRear dataRear;

	//附加信息
	_AppendInfo appendInfo;
public:
	tag_DataPacket() {
		memset(this, 0, sizeof(tag_DataPacket));
	}
}_DataPacket;

//系统暂停数据结构
typedef struct tag_PauseTime {
	//开始暂停时间
	int tstart;
	//结束暂停时间
	int tend;
	//对应pcap结构体的暂停时段ID
	int tid;
	tag_PauseTime() {
		memset(this, 0, sizeof(tag_PauseTime));
	}
}_PauseTime;

//pcap文件统计信息数据结构
typedef struct tag_Pcapfile {
	bool _paused = false;  //暂停标识
	bool _timefly = false;
	bool _broken = false;
	bool _gpsnormal = true;
	int _nametime = 0 ;
	int64_t _size = 0;
	int32_t _pkgmissedmax = 0;	//单次丢帧最大数量
	int64_t _packetmissed = 0;	//共丢多少帧
	std::string _path = "";
	tag_Pcapfile() {

	}
}_Pcapfile;
#pragma pack(pop)