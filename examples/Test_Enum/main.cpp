#include <iostream>
#include <magic_enum.h>
using namespace std;


enum class eNumber
{
	eOne,
	eTwo,
	eThree,
	eFour
};

int main(int argc, char** argv)
{

	auto  str = magic_enum::enum_name(eNumber::eFour);

	std::cout << typeid(str).name() << " " << str << std::endl;

	std::cout << "Enum Size : " << magic_enum::enum_count<eNumber>() << std::endl;


	auto names = magic_enum::enum_names<eNumber>();

	std::cout << "Enum Context : " << std::endl;
	for (auto name : names)
	{
		std::cout << name << " ";
	}
	std::cout << endl;

	std::cout << "Index : " << magic_enum::enum_index<eNumber>(eNumber::eThree).value() << std::endl;

	std::cout << "Has 10: " << magic_enum::enum_contains<eNumber>(1) << std::endl;

	
	return 0;
}