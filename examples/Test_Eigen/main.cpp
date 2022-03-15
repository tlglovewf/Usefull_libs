#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include <Cereal/archives/binary.hpp>
#include <Cereal/archives/xml.hpp>

using namespace std;

class MyData
{
public:
	MyData() = default;
	MyData(int _a, int _b) :a(_a), b(_b) {}
	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(a, b);
	}

	friend ostream& operator << (ostream & os, const MyData & data)
	{
		os << data.a << " " << data.b << endl;
		return os;
	}

protected:
	int a = 20;
	int b = 40;
};


int main(int argc, char** argv)
{
	Eigen::Matrix2f matrix;

	matrix << 0, 1, 2, 3;
	cout << matrix << endl;
	cout << "-----" << endl;
	cout << matrix.transpose() << endl;
	cout << "-----" << endl;
	cout << matrix.inverse() << endl;

	{
		/*******/
		ofstream file("test.bin");
		cereal::BinaryOutputArchive ar(file);
		//cereal::XMLOutputArchive ar(file);
		int a = 10;
		std::string b = "test";
		int c = 15;
		MyData data(5,10);
		ar(a, cereal::binary_data(b.c_str(),10),  c, data);
	}

	{
		ifstream file("test.bin");
		cereal::BinaryInputArchive ar(file);
		//cereal::XMLInputArchive ar(file);

		int a;
		char br[10] = { 0 };

		auto b = cereal::binary_data(br, 10);
		int c;
		MyData data;
		ar(a,b, c, data);
		
		std::cout << a << " " << br << " " << c << " " << data << std::endl;
	}

	return 0;
}
