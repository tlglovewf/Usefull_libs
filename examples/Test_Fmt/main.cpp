#include <iostream>
#include <vector>
#include <fmt/fmt.h>
#include "T_Functions.h"
using namespace std;

#include <algorithm>


#include <fstream>
#include <sstream>
#include <iomanip>
void rewritepcapfile(const std::string& file1, const std::string& file2)
{
	std::ifstream inf(file1);
	std::ofstream onf(file2);

	std::vector<std::string> values;

	while (!inf.eof())
	{
		std::string line;
		std::getline(inf, line);
		if (values.empty())
		{
			values.emplace_back(line); // add headline 
		}
		else
		{
			if (line.empty()) //maybe end
				continue;	
			std::stringstream ss(line);
			int d;
			std::string value;
			ss >> d >> value;
			values.emplace_back(value);
		}
	}

	if (!values.empty())
	{
		int index = 0;
		for (auto item : values)
		{
			if (index == 0)
				onf << item.c_str() << endl;
			else
				onf << std::left << std::setw(15)<< index << item.c_str() << endl;
			++index;
		}
   }

}


class Te
{
public:

	int t = 0;
	std::string v;

};
//vector中插入值
template<typename T>
void v_insert(std::vector<T>& rt, const std::vector<T>& ins, int index)
{
	index = std::min(std::max(index, 0), (int)rt.size());
	size_t orilen = rt.size();
	rt.resize(orilen + ins.size());
	if (index != orilen)
	{
		std::vector<int> temp(&rt[index], &rt[orilen]);

		memcpy(&rt[index + ins.size()], &temp[0], sizeof(T) * temp.size());
	}
	memcpy(&rt[index], &ins[0], sizeof(T) * ins.size());
}


int main(int argc, char** argv)
{


	std::cout.precision(30);

	std::cout << 180.0 / 3.14159265358979323846  << std::endl;

	return 0;

	std::vector<Te> te = { {10,"hello"}, {11,"world"} };
	std::vector<Te> hd = { {9,"No"},{7,"wd"} };

	//v_insert(te, hd, 2);

	for (auto item : te)
	{
		std::cout << item.t << " " << item.v << std::endl;
	}


	std::cout << fmt::format("{0}-{1}-{2}", 10, 7, 9) << std::endl;;
	//auto s0 = std::format("{:6}", 42);    // s0 的值为 "    42"
	//auto s2 = std::format("{:*<6}", 'x'); // s2 的值为 "x*****"
	//auto s3 = std::format("{:*>6}", 'x'); // s3 的值为 "*****x"
	//auto s4 = std::format("{:*^6}", 'x'); // s4 的值为 "**x***"
	std::cout << fmt::format("{0:-<10}{1:-<10}{2:<10}{3:<10}{4:<10}", 10.123, 33.3333, 10, 244333, "hello") << std::endl;

	//print +,- synar
	std::cout << fmt::format("{0:},{0:-},{0:+}", 1) << std::endl;


	//进制转换	 0*b 0*d 0*x===> *x表示字符位数
	int x = 120;
	std::cout << fmt::format("{0:#08b},{0:-#08d},{0:#08x}",x) << std::endl;

	//小数点
	double y = 12345.123456789;
	std::cout << fmt::format("{0:.2f},{0:.10f}", y) << std::endl;
	std::cout << fmt::format("{:{}f}", y, 2) << std::endl;

	//unicode
	std::cout << fmt::format("{0},{1}", "你", "好") << std::endl;

	

	std::cout << "--------------------------------------------------" << std::endl;

	std::cout << CommUtils::Functions::WriteLine<10,3>(10.12345, 12.54321, 13.11223) << std::endl;


	std::vector<int>  rts = { 1,2,3,4,5 };

	//v_insert(rts, {12,11,13}, 3);

	for (size_t i = 0; i < rts.size(); ++i)
	{
		if (rts[i] == 3)
		{
			v_insert(rts, { 13,23,33 }, i);
		}
	}

	for (auto i : rts)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;


	/*rewritepcapfile("E:\\Datas\\4201-1-00L099-220406\\RawData\\LiDAR\\LiDAR0\\pcapfile.txt",
					"E:\\Datas\\4201-1-00L099-220406\\RawData\\LiDAR\\LiDAR0\\pcap.txt");*/

	return 0;
}