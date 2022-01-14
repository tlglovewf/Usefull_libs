#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <vector>


template<typename ... T>
int sum(T ... t)
{
	return (t + ...);
}

TEST_CASE("This is first test case","[sum]")
{
	CHECK(sum(1, 2, 3) == 6);
	
	std::vector<int> test = { 1,3,4 };
	
	SECTION("test vector")
	{
		
		REQUIRE(test.size() == 3);
		CHECK(test.capacity() < 0);
	}
	CHECK(sum(1, 2, 3) == 5);
	WHEN("the size")
	{
		test.resize(10);
		THEN("check size")
		{
			REQUIRE(test.size() == 10);	 //require失败就会结束test case  check不会
		}
	}

	//浮点比较  eps 是数值百分比 margin是绝对值
	Approx target = Approx(100).epsilon(0.001);

	CHECK(100.0001 == target);
	CHECK(101.1 == target);

	//检查是否有异常
	//REQUIRE/CHECK_NOTHROW(XXX);

	//检查是否有yyy条件异常
	//REQUIRE / CHECK_THROWS_WITH(xxx, yyy);


	std::vector<int>   vecs = { 1,3,5,7,9 };
	using namespace Catch::Matchers;

	SECTION("Contains (element)")
	{
		CHECK_THAT(vecs, Catch::Matchers::UnorderedEquals(std::vector<int>{ 1,3,5,3,9 }));
	}
	
}