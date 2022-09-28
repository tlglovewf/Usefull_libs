#include <iostream>
#include <filesystem>
#include <fstream>
#include <bitset>
#include <leveldb/db.h>

using namespace std;

#if 1

bool FileEnctry(const std::string& file)
{
	filesystem::path fpath(file);

	filesystem::path opath = fpath.replace_filename("ot");

	return true;
}

int main(int argc, char** argv)
{
	std::bitset<8> bit(3);

	std::cout << bit.to_string() << std::endl;

	//leveldb::DB* pdb = nullptr;

	//leveldb::Options options;

	//options.create_if_missing = true;

	//leveldb::Status status = leveldb::DB::Open(options, "H:/test.db", &pdb);

	//if (status.ok())
	//{
	//	const std::string key  = "testforonce";
	//	const std::string value= "password";

	//	leveldb::Status result = pdb->Put(leveldb::WriteOptions(), key, value);

	//	std::string kvalue;
	//	result = pdb->Get(leveldb::ReadOptions(), key, &kvalue);

	//	std::cout << kvalue << std::endl;

	//}
	//else
	//{
	//	std::cout << "open db error." << std::endl;
	//}

	return 0;
}
#else


#include <string>
#include <filesystem>
#include <fstream>
#include <functional>

#pragma pack(1)
struct GSOF_31H//full nav info

{
	char            recordtype;
	char            recordlength;
	short           gpsweek;
	unsigned long   gpstime;
	char            imualig;
	char            gnssstatus;
	double          lat;
	double          lon;
	double          alt;
	float           northvel;
	float           eastvel;
	float           downvel;
	float           speed;
	double          roll;
	double          pitch;
	double          heading;
	double          trackangle;
	float           gyro_x; //degrees / sec
	float           gyro_y;
	float           gyro_z;
	float           acc_x;  //meters / sec~2
	float           acc_y;
	float           acc_z;
};

struct LvxDataSegment
{
	char stx;
	char status;
	char packettype;
	char length;
	char transnum;
	char pgindex;
	char pgmxindex;
	GSOF_31H gsofdata;
	char     checksum;
	char     etx;
};
#pragma pack()

template<typename T>
T SwitchByteStorage(T t)
{
	constexpr size_t len = sizeof(T);
	char ch[len];

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

/// <summary>
///  读取lvx dat文件
/// </summary>
/// <param name="path">路径</param>
/// <param name="func">单项处理函数</param>
void ReadLVXRawData(const std::string& path, std::function<void(const LvxDataSegment&)> func)
{
	if (!path.empty() && std::filesystem::exists(path))
	{
		std::ifstream  datfile(path, std::ios::binary);
		constexpr size_t  datasize = sizeof(GSOF_31H);
		constexpr size_t  recordsize = sizeof(LvxDataSegment);
		static_assert(datasize == 106/*单条总长度*/, "the full data len is error");
		constexpr char headmark = 0x02;//数据段头部标识

		std::string content;
		//读取文件全部信息
		content.assign(std::istreambuf_iterator<char>(datfile), std::istreambuf_iterator<char>());

		size_t index = 0;
		do
		{  //去噪
			while (content[index] != headmark)
				++index;

			LvxDataSegment item;
			memcpy((void*)(&item), (void*)(&content[index]), recordsize);

			//大小端转换
			item.gsofdata.gpsweek = SwitchByteStorage(item.gsofdata.gpsweek);
			item.gsofdata.gpstime = SwitchByteStorage(item.gsofdata.gpstime);
			item.gsofdata.lat = SwitchByteStorage(item.gsofdata.lat);
			item.gsofdata.lon = SwitchByteStorage(item.gsofdata.lon);
			item.gsofdata.alt = SwitchByteStorage(item.gsofdata.alt);
			item.gsofdata.northvel = SwitchByteStorage(item.gsofdata.northvel);
			item.gsofdata.eastvel = SwitchByteStorage(item.gsofdata.eastvel);
			item.gsofdata.downvel = SwitchByteStorage(item.gsofdata.downvel);
			item.gsofdata.speed = SwitchByteStorage(item.gsofdata.speed);
			item.gsofdata.roll = SwitchByteStorage(item.gsofdata.roll);
			item.gsofdata.pitch = SwitchByteStorage(item.gsofdata.pitch);
			item.gsofdata.heading = SwitchByteStorage(item.gsofdata.heading);
			item.gsofdata.trackangle = SwitchByteStorage(item.gsofdata.trackangle);
			item.gsofdata.gyro_x = SwitchByteStorage(item.gsofdata.gyro_x);
			item.gsofdata.gyro_y = SwitchByteStorage(item.gsofdata.gyro_y);
			item.gsofdata.gyro_z = SwitchByteStorage(item.gsofdata.gyro_z);
			item.gsofdata.acc_x = SwitchByteStorage(item.gsofdata.acc_x);
			item.gsofdata.acc_y = SwitchByteStorage(item.gsofdata.acc_y);
			item.gsofdata.acc_z = SwitchByteStorage(item.gsofdata.acc_z);
			index += recordsize;

			if (fabs(item.gsofdata.pitch) > 360.0)
				continue;//bad data
			if (func)
				func(item);


		} while (index < content.size());

	}
}
/// <summary>
/// 字符格式化输出
/// </summary>
/// <typeparam name="...Args">参数</typeparam>
/// <typeparam name="N">格式化宽度</typeparam>
/// <typeparam name="F">小数点位数</typeparam>
/// <typeparam name="...Args">参数</typeparam>
/// <returns></returns>
//template<int N, int F = 0, typename ... Args>
//static std::string WriteLine(Args ... args)
//{
//	size_t sz = sizeof...(args);
//
//	std::string _fmt;
//
//	for (size_t i = 0; i < sz; ++i)
//	{
//		std::string temp;
//		if (F != 0)
//		{
//			temp = "{" + std::to_string(i) /* + fmt::format(":<{0}.{1}f", N, F) */ + "}";
//		}
//		else
//		{
//			temp = "{" + std::to_string(i) /* + fmt::format(":<{:}", N)*/ +  "}";
//		}
//		_fmt += temp;
//	}
//	_fmt += "\n";
//	return fmt::format(_fmt, args...);
//}

/// <summary>
/// 周秒转天秒
/// </summary>
/// <param name="wktime"></param>
/// <returns></returns>
static inline double WkTime2DayTime(double wktime)
{
	return fmod(wktime, 86400);
}

int main(int argc, char** argv)
{

	std::cout << sizeof(unsigned long) << " " << sizeof(double) << " " << sizeof(float) << std::endl;
	return 0;
	const std::string infile = argv[1];
	const std::string ofile  = argv[2];

	std::ofstream sensor_imu(ofile);

	ReadLVXRawData(infile, [&sensor_imu](const LvxDataSegment& data) {
		auto imudata = std::move(data.gsofdata);

		double time = WkTime2DayTime(imudata.gpstime / 1000.0);

		std::string  line = std::to_string(imudata.lat) + "," + std::to_string(imudata.lon) + "," + std::to_string(time) + "\n";

		//std::string value = WriteLine<20, 10>(time,
		//	imudata.roll, imudata.pitch, imudata.heading,
		//	imudata.acc_x, imudata.acc_y, imudata.acc_z,
		//	imudata.gyro_x, imudata.gyro_y, imudata.gyro_z,
		//	imudata.lon, imudata.lat, imudata.alt);
		//	sensor_imu.write(value.c_str(), value.size());
		//});
		std::string value = std::to_string(time) + "," + std::to_string(imudata.acc_x) + "," + std::to_string(imudata.lat) + "\n";
		sensor_imu.write(value.c_str(), value.size());
		});
	return 0;
}




#endif