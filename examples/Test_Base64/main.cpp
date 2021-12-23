#include <iostream>
#include "base64.h"
#include <fstream>
#include <string>

using namespace std;


#define  ISENCODE 0
int main(int argc, char** argv)
{

	try
	{
#if ISENCODE
		ifstream  imgfile("E:\\Sources\\QMTDataFactory\\QtMainWindow\\Ui\\Image\\Navinfo\\generate_track.png",ios::binary);
#else
		ifstream  imgfile("base64.txt");
#endif
		string values;
		values.assign(istreambuf_iterator<char>(imgfile), istreambuf_iterator<char>());

#if ISENCODE	
		ofstream basefile("out.txt");

		std::string result = base64_encode(string_view(values));
#else
		ofstream basefile("set.png",ios::binary);

		std::string result = base64_decode(string_view(values));
#endif
		basefile << result;
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}