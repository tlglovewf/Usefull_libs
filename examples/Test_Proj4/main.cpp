#include <iostream>
#include <proj_api.h>
#include <cmath>
using namespace std;
#define WRONGDATA -1000 //错误值
#define PI 3.141592657
//! 经纬度
struct BLHCoordinate
{
	double lon;

	double lat;

	double alt;

	BLHCoordinate(double _lat, double _lon, double _alt) : lon(_lon), lat(_lat), alt(_alt) {}

	BLHCoordinate() : alt(WRONGDATA) {}

	//有效性
	static inline bool isValid(const BLHCoordinate& blh)
	{
		return blh.alt > WRONGDATA;
	}
};

struct Datum
{
	double r_max;
	double r_min;
	double e2;
};


Datum WGS84Datum{ 6378137, 6356752.314245, 0.00669437999013 };

struct Point3d
{
	double x;
	double y;
	double z;
};
#define D2R(X) X * DEG_TO_RAD
//! 经纬度转高斯投影坐标（B 维度  L 精度   H 高程)
static Point3d BLH_to_GaussPrj(const BLHCoordinate& BLH, Datum datum = WGS84Datum) //,double lon)
{
	int ProjNo, ZoneWide; ////带宽
	double longitude0, X0, xval, yval;
	double a, e2, ee, NN, T, C, A, M, b, l; //, h;
	b = BLH.lat * RAD_TO_DEG;
	l = BLH.lon * RAD_TO_DEG;
	//    h = BLH.z;
	ZoneWide = 3; //3度带宽
	a = datum.r_max;
	ProjNo = (int)((l - 1.5) / ZoneWide + 1); //计算度数号
	std::cout << ZoneWide << " " << BLH.lon << std::endl;
	longitude0 = ProjNo * ZoneWide; //中央经线

	longitude0 = D2R(longitude0);
	l = D2R(l); //经度转换为弧度
	b = D2R(b); //纬度转换为弧度
	e2 = datum.e2;
	ee = e2 * (1.0 - e2);
	NN = a / sqrt(1.0 - e2 * sin(b) * sin(b));
	T = tan(b) * tan(b);
	C = ee * cos(b) * cos(b);
	A = (l - longitude0) * cos(b);
																		    
	M = a * ((1 - e2 / 4 - 3 * e2 * e2 / 64 - 5 * e2 * e2 * e2 / 256) * b - (3 * e2 / 8 + 3 * e2 * e2 / 32 + 45 * e2 * e2 * e2 / 1024) * sin(2 * b) + (15 * e2 * e2 / 256 + 45 * e2 * e2 * e2 / 1024) * sin(4 * b) - (35 * e2 * e2 * e2 / 3072) * sin(6 * b));
	xval = NN * (A + (1 - T + C) * A * A * A / 6 + (5 - 18 * T + T * T + 72 * C - 58 * ee) * A * A * A * A * A / 120);
	yval = M + NN * tan(b) * (A * A / 2 + (5 - T + 9 * C + 4 * C * C) * A * A * A * A / 24 + (61 - 58 * T + T * T + 600 * C - 330 * ee) * A * A * A * A * A * A / 720);
	X0 = 500000L;
	xval = xval + X0;
	cout.precision(10);
	cout << yval << " " << xval << " " << BLH.alt << endl;
	return { yval, xval, BLH.alt };
}
class GuassMath final
{
public:
	//计算中央精度线
	static int CalcCentralMeridian(double lon)
	{
		constexpr int dot = 6;
		int d = lon / dot;
		return (d + 1) * dot - 3;
	}
	//计算中央精度线
	static int CalcCentralMeridian(int bandno, int band)
	{
		if (band == 3)
		{
			return 3 * bandno;
		}
		else if (band == 6)
		{
			return 6 * bandno - 3;
		}
		else
		{
			return -1;
		}
	}
	//计算中央精度线
	static int CalcDegreeBandNo(double lon, int band)
	{
		if (band == 3)
		{
			return (lon + 1.5) / 3;
		}
		else if (band == 6)
		{
			return (lon + 6) / 6;
		}
		else
		{
			return -1;
		}
	}

	 
protected:
	GuassMath() = default;
};

#define EARTH_RADIUS 6378137
#define PI           3.141592657

#define D2R(X)      ((X) * 0.0174532925388889)

//经度转墨卡托
static double lon2Mercator(double lon) {
	return D2R(lon) * EARTH_RADIUS;
}

//纬度转墨卡托
 static double lat2Mercator(double lat) {
	double y = log(tan((90 + lat) * PI / 360)) / (PI / 180);
	return D2R(y) * EARTH_RADIUS;
}

int main(int argc, char** argv)
{
	std::cout.precision(15);	  
	std::cout << lon2Mercator(-74.564531) << " " << lat2Mercator(40.663508) << std::endl;
	std::cout << lon2Mercator(-74.564528) << " " << lat2Mercator(40.663506) << std::endl;
	
	getchar();
	return 0;

	DEG_TO_RAD;

	projPJ  wgsprj = pj_init_plus("+proj=longlat +datum=WGS84 +no_defs");

	if (!wgsprj)
		exit(-1);
	//epsg(4326 wgs84) (31467 Gauss-Kruger zone 3)
    //merc:mercator	  "+proj=merc +lon_0=0 +k=1 +x_0=0 +y_0=0 +ellps=WGS84 +datum=WGS84 +units=m +no_defs"
	const char* mecstr = "+proj=merc +lon_0=0 +k=1 +x_0=0 +y_0=0 +ellps=WGS84 +datum=WGS84 +units=m +no_defs";
	//geocent:ecef	  "+proj=geocent +lon_0=0 +k=1 +x_0=0 +y_0=0 +ellps=WGS84 +datum=WGS84 +units=m +no_defs"
	const char* ecef   = "+proj=geocent +lon_0=0 +k=1 +x_0=0 +y_0=0 +ellps=WGS84 +datum=WGS84 +units=m +no_defs";
	//gauss:		  "+proj=tmerc +lat_0=0 +lon_0=9 +k_0=1 +x_0=3500000 +y_0=0 +datum=WGS84 +units=m"
	//lon_0 度带  x_0 偏移 
	const char* gauss  = "+proj=tmerc +lat_0=0 +lon_0=114 +k_0=1 +x_0=500000 +y_0=0 +datum=WGS84 +units=m";
	projPJ  dstprj = pj_init_plus(gauss);

	if (!dstprj)
		exit(-1);
	std::cout.precision(10);
	double lon = 114.47185633518 * DEG_TO_RAD;
	double lat =  30.44960450003 * DEG_TO_RAD;
	double alt = 0;

	BLH_to_GaussPrj({ lat,lon,alt });
	int result = pj_transform(wgsprj, dstprj, 1, 1, &lon, &lat, &alt);
	
	std::cout << lat << " " << lon << " " <<  alt << std::endl;




	pj_free(wgsprj);
	pj_free(dstprj);

	return 0;
}