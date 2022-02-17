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
	REQUIRE(func(1)); //检查不通过 不再检查后续
	CHECK(func(0));	  //检查不通过 记录继续后续
	CHECK(func(6));
}