#include <iostream>
#include <fstream>

//#include <Json/include/nlohmann/json.hpp>

#include <Json/single_include/nlohmann/json.hpp>

using namespace std;

using json = nlohmann::json;

int main(int argc, char** argv)
{

	json j = R"(
				{
				"wello":1,
				"horld":2,
				"array":[1,2,3,4,5]
				}
				)"_json;  //_json must be add 

	std::cout << j << std::endl;
	std::cout << j["wello"].get<int>() << std::endl;
	std::cout << j["array"] << std::endl;

	auto item = j["array"].get<std::vector<int> >();
	std::cout << item[3] << std::endl;
	j["wello"] = 10;
	std::cout << j << std::endl;

	//----------------------R/W Json-------------------------//
	
	std::ifstream  infile("test.json");

	infile >> j;

	std::cout << j << std::endl;

	std::ofstream ofile("out.json");

	j["sampleRate"] = 10;
	//格式化输出
	ofile << std::setw(4) << j << std::endl;


	getchar();
	return 0;
}