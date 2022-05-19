#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
	std::string time("072644");
	int shour = atoi(time.substr(0, 2).c_str());
	int smint = atoi(time.substr(2, 2).c_str());
	int ssecs = atoi(time.substr(4, 2).c_str());

	std::cout << shour * 3600 + smint * 60 + ssecs << std::endl;

	return 0;
}