#pragma once

class Test
{
public:
	Test() {}
	Test(int a, int b) :_x(a), _y(b) {}

	void display()const noexcept
	{
		std::cout << _x << " " << _y << std::endl;
	}
private:
	int _x = 0;
	int _y = 0;
	inline static int _in_var = 10;//内联变量 可以在头文件中初始化static变量
};