#include <iostream>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
using namespace std;

bool func(int n)
{
	return n > 1;
}

TEST_CASE("hello world")
{
	REQUIRE(func(1)); //��鲻ͨ�� ���ټ�����
	CHECK(func(0));	  //��鲻ͨ�� ��¼��������
	CHECK(func(6));
}