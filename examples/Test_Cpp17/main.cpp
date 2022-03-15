#include <iostream>
#include <tuple>
#include <set>
#include <map>
#include <variant>
#include <optional>
#include <string>
#include <memory>
#include <any>
#include <filesystem>

#include "test.h"
#include "T_FileSystem.h"
#include "T_Random.h"
using namespace std;


std::tuple<int, double> getfunc()
{
	return std::tuple(10, 22.2);
}


//�ṹ����
void test_strcutbind()
{
	std::pair test(10, 22.2);//�Զ��Ƶ� pair<int,double>

	auto [i, d] = getfunc();//�ṹ���� �Զ��Ƶ�tuple������ֵ
	std::cout << i << " " << d << std::endl;

	auto &[m, n] = test;
	n = 33.3;
	std::cout << m << " " << n << std::endl;

	std::map<int, std::string> mp =
	{
		{10,"one"},
		{11,"two"},
	};
	for (const auto &[x,y] : mp)
	{
		std::cout << x << " " << y << std::endl;
	}

	//�����԰�����ͽṹ��
	int array[] = { 1,2,3 };
	auto [a, b, c] = array;
	std::cout << a << " " << b << " " << c << std::endl;
}

int getValue()
{
	int v;
	std::cout << "Compare with 10,Please input Numb: " << std::endl;
	cin >> v;
	return v;
}
//17 if-switch ���ʽ
void test_ifswitch()
{
	if (int t = getValue(); t < 10)
	{
		std::cout << "less" << std::endl;
	}
	else
	{
		std::cout << "great" << std::endl;
	}
}

//�۵�
template<typename ... Ts >
auto Sum(Ts ... ts)
{
	return (ts + ...);
}

//constexpr ����ʱ�� lambda
//�����岻�ܰ��������䡢goto��䡢label��try�顢��̬�������ֲ߳̾��洢��
//û�г�ʼ������ͨ���������ܶ�̬�����ڴ棬������new delete�ȣ������麯��
void test_constlambda()
{
	constexpr auto lamb = [](int n) {return n * n; };
	static_assert(lamb(3) == 9, "a");
}


namespace One::Two::Three
{
	struct Pt{};
}

//�ؼ���
#if defined __has_include
#if !__has_include("test.h") // �ж��Ƿ�������ͷ�ļ�
#error "test"
#endif
#endif

//��� ��ʹ�÷���ֵ ������
[[nodiscard]] int dofunc()
{
	return 10;
}
void test_Attrib()
{
	int d;
	std::cout << "input value:" << std::endl;
	cin >> d;
	switch (d)
	{
	case 1:
		//add more
		std::cout << "get" << std::endl;
		[[fallthrough]]; //���ξ��棿
	case 2:
		//[[fallthrough]]; //���ξ��棿
	default:
		break;
	}
	dofunc();
	[[maybe_unused]] //����δʹ�ñ�������
	int tts;
}

//���߼���union
void test_variant()
{
	std::variant<int, std::string> test("test");
	std::cout << test.index() << std::endl;
	std::cout << std::get<std::string>(test) << std::endl;
	test = 10;
	std::cout << std::get<0>(test) << std::endl;

	std::variant<std::monostate,int, std::string> var;//ģ���״̬
	var = 10;
}


std::optional<int> StoI(const std::string& str)
{
	try
	{
		return std::stoi(str);
	}
	catch (...)
	{
		return std::nullopt;
	}
}

void test_optional()
{
	std::string ss("d123");
	auto o = StoI(ss);
	if (o)
	{
		std::cout << *o << std::endl;
	}
	else
	{
		std::cout << "error numb." << std::endl;
	}
}

//any �洢�κε�ֵ
void test_any()
{
	std::any a = Test(10, 23);

	std::cout << a.type().name() << std::endl;

	auto b = std::any_cast<Test>(a);

	b.display();

}

//apply ����ֱ�ӽ��
int add(int first, int second) { return first + second; }
void test_apply()
{
	std::cout << std::apply(add, std::pair(10, 32)) << std::endl;

	auto add_lambda = [](auto first, auto second) { return first + second; };

	std::cout << std::apply(add_lambda,std::tuple(33.2f,22.3f)) << std::endl;
}


void test_make_from_tuple()
{
	auto tup = std::make_tuple(32, 22);
	auto t = std::make_from_tuple<Test>(std::move(tup));
	t.display();
}

namespace fs = std::filesystem;

void test_filesystem()
{
	fs::path filepath("E:/CamCalibra/data/Ref/MATLAB_Calculate/Result/10.jpg");

	if (fs::exists(filepath))
	{
		std::cout << "file size : " << fs::file_size(filepath) << std::endl;  //�ֽ�
		std::cout << "exist." << std::endl;
		
		fs::directory_entry entry(filepath);
		auto status = entry.status();

		if (entry.is_directory())
		{
			
			fs::directory_iterator list(entry);
			for (auto& it : list)
			{
				std::cout << it.path().filename() << std::endl;
			}
		}
		else
		{
			std::cout << "it is a file" << std::endl;
			std::cout << entry.path().filename() << std::endl;
		}
	}
	else
	{
		std::cout << "not exist." << std::endl;
	}
}

enum Te
{
	eOne,
	eTwo
};

#include "T_Timer.h"



//void* operator new[](size_t size)
//{
//	cout << "new []" << endl;
//	return nullptr;
//}
void operator delete[](void *d)
{
	cout << "delete []" << endl;
}

void operator delete(void* d)
{
	cout << "delete" << endl;
}

template<typename ... Args>
std::string format(const std::string& pattern, Args ...args)
{
	size_t size = 1 + snprintf(nullptr, 0, pattern.c_str(), args ...);
	std::unique_ptr<char[]> buffer(new char[size]) ;
	snprintf(buffer.get(), size, pattern.c_str(), args ...);
	return std::string(buffer.get());
}

int main(int argc, char** argv)
{
	//test_strcutbind();
	//
	//test_ifswitch();
	//
	//std::cout << Sum(10, 12, 32) << std::endl;
	//
	//One::Two::Three::Pt pt;//�����ռ�Ƕ��
	//
	//
	//test_variant();
	//
	//test_optional();
	//
	//test_any();
	//
	//test_apply();
	//
	//test_make_from_tuple();

	//test_filesystem();

	//if (fs::remove("E:/tools/test.txt"))
	//{
	//	std::cout << "successfully." << std::endl;
	//}
	
	//int value = 1'000'000;
	//
	//std::cout << value << std::endl;

	auto str = format("%s%d", "���", 100);

	std::cout << str.c_str() << std::endl;
	return 0;
}