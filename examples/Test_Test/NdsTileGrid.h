#pragma once

struct GridBound
{
	double _minx;
	double _miny;
	double _maxx;
	double _maxy;
	GridBound() { memset(this, 0, sizeof(*this)); }
};
#define DEFAULTNDSLEVEL 13
class NdsTileMethod
{
public:

	/*
	 * 根据经纬度获取ndstileid
	 */
	template<int N = DEFAULTNDSLEVEL>
	static int GetTileId(double lon, double lat)
	{
		int nTileID;
		int p;
		int nX = (int)(lon / 90 * (1 << 30));
		int nY = (int)(lat / 90 * (1 << 30));

		nTileID = nX < 0 ? 1 : 0;

		for (p = 30; p > (30 - N); p--) {
			nTileID <<= 1;
			if ((nY & (1 << p)) != 0) {
				nTileID |= 1;
			}
			nTileID <<= 1;
			if ((nX & (1 << p)) != 0) {
				nTileID |= 1;
			}
		}
		nTileID += (1 << (16 + N));

		return nTileID;
	}

	/// <summary>
	/// 转二进制字符串
	/// </summary>
	/// <param name="value">int值</param>
	/// <param name="reverse">是否倒序输出</param>
	/// <returns></returns>
	static std::string Inter2BinaryString(int value, bool reverse = false)
	{
		std::string result;
		while (value)
		{
			if (value & 0x01)
				result.push_back('1');
			else
				result.push_back('0');
			value >>= 1;
		}
		if (reverse)
			return result;
		else
			return std::string(result.rbegin(), result.rend());
	}

	/// <summary>
	/// 二进制字符串转int
	/// </summary>
	/// <param name="v">二进制字符串</param>
	/// <returns>int值</returns>
	static int BinaryString2Inter(const std::string& v)
	{

		if (v.empty())
			return 0;
		else
		{
			int result = 0;
			int len = v.size() - 1;
			for (int i = len; i >= 0; --i)
			{
				result |= (((v[i] == '0') ? 0 : 1) << (len - i));
			}
			return result;
		}
	}

	/// <summary>
	/// 根据tileid号获取tile瓦片范围
	/// </summary>
	/// <param name="tileid">tile号</param>
	/// <returns></returns>
	template<int N = DEFAULTNDSLEVEL>
	static GridBound GetBoundFromTileid(long tileid)
	{
		std::string left = Inter2BinaryString(tileid, true);

		left.resize(2 * N + 1);
		std::string x, y;

		for (size_t i = 0; i < left.size(); ++i)
		{
			if (i % 2 == 0)
			{
				x = left[i] + x;
			}
			else
			{
				y = left[i] + y;
			}
		}

		double tileSize = 180 / pow(2, N);

		int tileX = BinaryString2Inter(x);
		int tileY = BinaryString2Inter(y);

		GridBound grid;
		grid._minx = tileX * tileSize;
		grid._miny = tileY * tileSize;
		grid._maxx = grid._minx + tileSize;
		grid._maxy = grid._miny + tileSize;

		std::cout << grid._minx << " " << grid._maxx << std::endl;
		std::cout << grid._miny << " " << grid._maxy << std::endl;
		return grid;
	}
};
