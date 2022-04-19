#pragma once

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
#pragma pack(pop)

template<typename T>
T SwitchByteStorage(T t)
{
	constexpr size_t len = sizeof(T);
	char ch[len];
	char dst[len];
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
			item.gsofdata.gpsweek	 = SwitchByteStorage(item.gsofdata.gpsweek	  );
			item.gsofdata.gpstime	 = SwitchByteStorage(item.gsofdata.gpstime	  );
			item.gsofdata.lat		 = SwitchByteStorage(item.gsofdata.lat		  );
			item.gsofdata.lon		 = SwitchByteStorage(item.gsofdata.lon		  );
			item.gsofdata.alt		 = SwitchByteStorage(item.gsofdata.alt		  );
			item.gsofdata.northvel	 = SwitchByteStorage(item.gsofdata.northvel	  );
			item.gsofdata.eastvel	 = SwitchByteStorage(item.gsofdata.eastvel	  );
			item.gsofdata.downvel	 = SwitchByteStorage(item.gsofdata.downvel	  );
			item.gsofdata.speed		 = SwitchByteStorage(item.gsofdata.speed	  );
			item.gsofdata.roll		 = SwitchByteStorage(item.gsofdata.roll		  );
			item.gsofdata.pitch		 = SwitchByteStorage(item.gsofdata.pitch	  );
			item.gsofdata.heading	 = SwitchByteStorage(item.gsofdata.heading	  );
			item.gsofdata.trackangle = SwitchByteStorage(item.gsofdata.trackangle );
			item.gsofdata.gyro_x	 = SwitchByteStorage(item.gsofdata.gyro_x	  );
			item.gsofdata.gyro_y	 = SwitchByteStorage(item.gsofdata.gyro_y	  );
			item.gsofdata.gyro_z	 = SwitchByteStorage(item.gsofdata.gyro_z	  );
			item.gsofdata.acc_x		 = SwitchByteStorage(item.gsofdata.acc_x	  );
			item.gsofdata.acc_y		 = SwitchByteStorage(item.gsofdata.acc_y	  );
			item.gsofdata.acc_z		 = SwitchByteStorage(item.gsofdata.acc_z	  );
			if (func)
				func(item);
			
			index += recordsize;
		} while (index < content.size());

	}
}


