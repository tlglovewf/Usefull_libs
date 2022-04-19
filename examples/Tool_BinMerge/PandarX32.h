#pragma once
#include <memory>

#pragma pack(push)
#pragma pack(1)
//ɨ���ǵ������ݽṹ
typedef struct tag_DataPoint
{
	unsigned short distance;
	unsigned char calref;
	char reserved;			//XT32 Ԥ��1�ֽ�
public:
	tag_DataPoint() {
		memset(this, 0, sizeof(tag_DataPoint));
	}
}_DataPoint;

//ɨ���ǵ������ݽṹ
typedef struct tag_DataBlock
{
	//unsigned short flag;
	unsigned short azimuth;
	//����ṹ
	_DataPoint datapoints[32];
public:
	tag_DataBlock() {
		//flag = (short)0xFFEE;
		memset(this, 0, sizeof(tag_DataBlock));
	}
}_DataBlock;

//XT32 ��ͷ
typedef struct tag_PacketHeader {
	char packetHeader[6];
public:
	tag_PacketHeader() {
		memset(this, 0, sizeof(tag_PacketHeader));
	}
}_PacketHeader;

//XT32 ����ͷ
typedef struct tag_DataHeader {
	char dataHeader[6];
public:
	tag_DataHeader() {
		memset(this, 0, sizeof(tag_DataHeader));
	}
}_DataHeader;

//XT32 ����β��
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

//������Ϣ
typedef struct tag_AppendInfo {

	//UDP Sequence �����ݰ������кţ���1 ��0xFF FF FF FF��С�˽���
	unsigned int udpSeq;
public:
	tag_AppendInfo() {
		memset(this, 0, sizeof(tag_AppendInfo));
	}
}_AppendInfo;

//ɨ���ǵ������ݰ����ݽṹ
typedef struct tag_DataPacket
{
	//����ͷ
	char header[42];

	//��ͷ
	_PacketHeader packetHeadr;

	//����ͷ
	_DataHeader dataHeader;

	//����ṹ
	_DataBlock datablocks[8];

	//����β
	_DataRear dataRear;

	//������Ϣ
	_AppendInfo appendInfo;
public:
	tag_DataPacket() {
		memset(this, 0, sizeof(tag_DataPacket));
	}
}_DataPacket;

//ϵͳ��ͣ���ݽṹ
typedef struct tag_PauseTime {
	//��ʼ��ͣʱ��
	int tstart;
	//������ͣʱ��
	int tend;
	//��Ӧpcap�ṹ�����ͣʱ��ID
	int tid;
	tag_PauseTime() {
		memset(this, 0, sizeof(tag_PauseTime));
	}
}_PauseTime;

//pcap�ļ�ͳ����Ϣ���ݽṹ
typedef struct tag_Pcapfile {
	bool _paused = false;  //��ͣ��ʶ
	bool _timefly = false;
	bool _broken = false;
	bool _gpsnormal = true;
	int _nametime = 0 ;
	int64_t _size = 0;
	int32_t _pkgmissedmax = 0;	//���ζ�֡�������
	int64_t _packetmissed = 0;	//��������֡
	std::string _path = "";
	tag_Pcapfile() {

	}
}_Pcapfile;
#pragma pack(pop)