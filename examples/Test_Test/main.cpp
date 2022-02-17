//#define CATCH_CONFIG_MAIN
//#include <catch2/catch.hpp>
//#include <vector>
//#include <bitset>
//#include "NdsTileGrid.h"
//template<typename ... T>
//int sum(T ... t)
//{
//	return (t + ...);
//}
//
//#include <stdlib.h>
//#define LONBITLEN   32
//#define LATBITLEN   31
//#define TOTALBITLEN 63
///// <summary>
///// 莫斯顿码
///// </summary>
///// <param name="x">经度二进制编码</param>
///// <param name="y">维度二进制编码</param>
///// <returns></returns>
//std::string mortoncode(const std::string& x, const std::string& y)
//{
//	std::string rx ;
//	std::string ry ;
//	//x 要满足32位长
//	if (x.size() < LONBITLEN)
//	{
//		int sift = LONBITLEN - x.size();
//		
//		rx.assign(sift,'0');
//
//		rx += x;
//	}
//	else
//	{
//		rx = x;
//		rx.resize(LONBITLEN);
//	}
//	//y 要31位长
//	if (y.size() < LATBITLEN)
//	{
//		int sift = LATBITLEN - y.size();
//
//		ry.assign(sift, '0');
//
//		ry += y;
//	}
//	else
//	{
//		ry = y;
//		ry.resize(LATBITLEN);
//	}
//		
//
//	std::string result;
//	result.resize(LONBITLEN + LATBITLEN);
//	int xiter = 0;
//	int yiter = 0;
//	int index = 0;
//	//最终结果是32 + 31 = 63 位长二进制编码
//	for (;xiter < rx.size();)
//	{
//		result[index++] = rx[xiter++];
//		result[index++] = ry[yiter++];
//	}
//	
//	return result;
//}
//
///// <summary>
///// 经度转int32
///// </summary>
///// <param name="lon"></param>
///// <returns></returns>
//std::string lon2int32(double lon)
//{
//	return std::bitset<32>(floor(2147483648/*pow(2,31)*/ * lon / 180.0)).to_string();
//}
///// <summary>
///// int32 转经度
///// </summary>
///// <param name="value"></param>
///// <returns></returns>
//double lonfromint32(const std::string& value)
//{
//	std::bitset<LONBITLEN> v(value);
//	long lon = v.to_ulong();
//
//	return lon * 180.0 / 2147483648/*pow(2,31)*/;
//}
//
///// <summary>
///// 纬度转int32
///// </summary>
///// <param name="lat"></param>
///// <returns></returns>
//std::string lat2int32(double lat)
//{
//	return std::bitset<31>(floor(1073741824 /*pow(2,30)*/ * lat / 90.0)).to_string();
//}
//
///// <summary>
///// int32 转纬度
///// </summary>
///// <param name="value"></param>
///// <returns></returns>
//double latfromint32(const std::string& value)
//{
//	std::bitset<LATBITLEN> v(value);
//	long lat = v.to_ulong();
//
//	return lat * 90.0 / 1073741824 /*pow(2,30)*/;
//}
//
//long calctileidfromlonlat(double lon, double lat, int level)
//{
//	std::string slon = lon2int32(lon);
//	std::string slat = lat2int32(lat);
//
//	std::string code = mortoncode(slon, slat);
//
//	int len = 2 * level + 1;  
//
//	std::string value(code.begin(), code.begin() + len);
//
//	return std::bitset<40>(value).to_ulong();
//}
//
//
//
//
//TEST_CASE("This is first test case","[sum]")
//{
//
//
//	//556467521
//	//std::cout << NdsTileMethod::GetTileId(116.4037085, 39.9119088) << std::endl;
//	/*std::cout << NdsTileMethod::toBinary<31>(10) << std::endl;*/
//
//	//NdsTileMethod::GetBoundFromTileId(556467521);
//
//	//std::cout << NdsTileMethod::GetTileWidth(13) << " " << NdsTileMethod::GetTileHeight(13) << std::endl;
//	//return;
//
//	//std::string result = mortoncode(lon2int32(2.2945), lat2int32(48.858222)); //"1101000011011001101110011", "100010101111100101111000101101");
//	//std::cout << result << std::endl;
//
//	//std::string x, y;
//	//
//	//demortoncode(result, x, y);
//	//
//	//std::cout << x << std::endl;
//	//std::cout << y << std::endl;
//
//
//	//std::cout << lonfromint32(x) << " " << latfromint32(y) << std::endl;
//	//std::cout << calctileidfromlonlat(2.2945, 48.858222, 13) << std::endl;
//	//
//
//	//getlonlatfromtileid(8428777, lon, lat);
//	//std::cout << lon << " " << lat << std::endl;
//	//getlonlatfromtileid(8428778, lon, lat);
//	//std::cout << lon << " " << lat << std::endl;
//	//getlonlatfromtileid(8428779, lon, lat);
//	//std::cout << lon << " " << lat << std::endl;
//
//	//const int tileid = 557467521;
//	  const int tileid = 556154711;
//	//getlonlatfromtileid(tileid - 1, lon, lat);
//	//std::cout << lon << " " << lat << std::endl;
//	  NdsTileMethod::GetBoundFromTileid(tileid);
//	//std::cout << lon << " " << lat << std::endl;
//	//getlonlatfromtileid(tileid + 1, lon, lat);
//	//std::cout << lon << " " << lat << std::endl;
//	//getlonlatfromtileid(tileid + 2, lon, lat);
//	//std::cout << lon << " " << lat << std::endl;
//	//getlonlatfromtileid(tileid + 3, lon, lat);
//	//std::cout << lon << " " << lat << std::endl;
//	return;
//
//	CHECK(sum(1, 2, 3) == 6);
//	
//	std::vector<int> test = { 1,3,4 };
//	
//	SECTION("test vector")
//	{
//		
//		REQUIRE(test.size() == 3);
//		CHECK(test.capacity() < 0);
//	}
//	CHECK(sum(1, 2, 3) == 5);
//	WHEN("the size")
//	{
//		test.resize(10);
//		THEN("check size")
//		{
//			REQUIRE(test.size() == 10);	 //require失败就会结束test case  check不会
//		}
//	}
//
//	//浮点比较  eps 是数值百分比 margin是绝对值
//	Approx target = Approx(100).epsilon(0.001);
//
//	CHECK(100.0001 == target);
//	CHECK(101.1 == target);
//
//	//检查是否有异常
//	//REQUIRE/CHECK_NOTHROW(XXX);
//
//	//检查是否有yyy条件异常
//	//REQUIRE / CHECK_THROWS_WITH(xxx, yyy);
//
//
//	std::vector<int>   vecs = { 1,3,5,7,9 };
//	using namespace Catch::Matchers;
//
//	SECTION("Contains (element)")
//	{
//		CHECK_THAT(vecs, Catch::Matchers::UnorderedEquals(std::vector<int>{ 1,3,5,3,9 }));
//	}
//	
//}