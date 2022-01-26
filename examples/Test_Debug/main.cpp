#include <iostream>
#include <map>
#include <string>
using namespace std;


namespace Test
{
	class Hello
	{
	public:
		Hello(int a) :_a(a) {}

		Hello& operator++()
		{
			++_a;
			return  *this;
		}

		 operator int()
		{
			return _a;
		}
	private:
		int _a;
	};
}


int main(int argc, char** argv)
{
	std::map<int, std::string>  values;
	values.insert({ 10,"hello" });
	values.insert({ 11,"world" });
	values.insert({ 12,"taxis" });

	if (int sz = values.size(); sz > 0)
	{
		for ( auto& item : values)
		{
			std::cout << std::apply([](int a, std::string& b)->std::string {return std::to_string(a) + b; }, item) << " ";
		}
	}

	Test::Hello world(10);

	std::cout << ++world << std::endl;
	getchar();
	return 0;
}