#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench/nanobench.h>
#include <FakeIt/standalone/fakeit.hpp>
#include <vector>
#include <bitset>

//����׮����
struct SomeInterface 
{
	virtual int foo(int) = 0;
	virtual int func()   = 0;
};

TEST_CASE("First Test Example.")
{
	std::vector<int> rt(5);

	REQUIRE(rt.size() == 5);  //��� �����ж�
	CHECK(rt.size() == 5);    //��� ���ж�

	
	SUBCASE(">>>>====>  Test In Sub Class")
	{
		rt.emplace_back(1);
		CHECK(rt.capacity() < 3);
	}

	SUBCASE(">===>>>>>  Test In Other Class")
	{
		CHECK(rt.size() == 6);
	}

	fakeit::Mock<SomeInterface> mock;

	//���ô�����ֵ
	fakeit::When(Method(mock, foo)).Return(1,2,3);

	SomeInterface& sm = mock.get();

	CHECK(sm.foo(1) == 2);
	CHECK(sm.foo(1) == 2);
	CHECK(sm.foo(1) == 2);

	//������������
	Method(mock, foo) * 2;

	//foo �Ƿ��б�����
	fakeit::Verify(Method(mock, foo));
	//foo �Ƿ���ȷ����
	//fakeit::Verify(Method(mock, foo)).Exactly(3);
	//sm.foo(2) �Ƿ񱻵���  ������Լ��϶���Ĵ�������
	//fakeit::Verify(Method(mock, foo).Using(2));
	// so on 

	double d = 0.1;
	ankerl::nanobench::Bench().run("First test.", [&] {
		d += 1.0 / d;
		if (d > 5.0)
		{
			d -= 5.0;
		}
		ankerl::nanobench::doNotOptimizeAway(d);
		});

}

