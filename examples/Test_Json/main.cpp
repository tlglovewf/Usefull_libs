#include <iostream>
#include <simdjson.h>
#include "yyjson.h"
#include <fstream>
using namespace std;


int main(int argc, char** argv)
{

	const char* jsonpath = "E:/Sources/QMTDataFactory/x64/Release/TypeL/rinexcfg.json";
	yyjson_read_err errinfo;
	yyjson_read_flag readflag = 0;
	yyjson_doc* pdoc = yyjson_read_file(jsonpath, readflag, nullptr, &errinfo);
	if (nullptr == pdoc)
	{
		std::cout << errinfo.msg << std::endl;
	}
	
	if (yyjson_write_file("rst.json", pdoc, YYJSON_WRITE_NOFLAG, nullptr, nullptr))
	{
		std::cout << "write successfully." << std::endl;
	}


	/*
	 * On Demand is a new, faster simdjson API with all the ease-of-use you are used to.
	 * While it provides a familiar DOM interface, under the hood it is different: it is parsing values as you use them. 
	 * With On Demand, you do not waste time parsing JSON you do not use, and you do not pay the cost of generating an intermediate DOM tree.
	 */
	//simdjson::dom::document doc;
	//
	//simdjson::dom::parser parser;
	//
	//simdjson::dom::element result = parser.load(jsonpath);
	
	//simdjson::ondemand::parser onparser;
	//simdjson::padded_string json = simdjson::padded_string::load(jsonpath);
	//simdjson::ondemand::document jsons = onparser.iterate(json);
	//
 	//
	//;
	//std::cout << jsons["rinexcfg"]["observationsuffix"] << std::endl;
	//std::cout << typeid(jsons["rinexcfg"]["observationsuffix"]).name() << endl;
	
	getchar();
	return 0;
}