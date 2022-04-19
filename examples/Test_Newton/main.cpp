#include <iostream>
using namespace std;



template<typename T>
auto NewtonMethod(T init_x, T thred)
{
	// 解 x^2 = 8
	// 则 f(x) = x ^ 2 - 8  f'(x) = 2 * x
	// x - (f(x)/f'(x))
	// 牛顿迭代公式  Xn+1 = Xn - (f(X)/f'(X))
	auto rst = init_x - ((init_x * init_x  - 8)/ (2 * init_x));
	if (fabs( rst * rst - 8) < thred)
		return rst;
	else
		return NewtonMethod(rst, thred);
}

int main(int argc, char** argv)
{

	cout << NewtonMethod(15.0, 1e-5) << endl;
	return 0;
}