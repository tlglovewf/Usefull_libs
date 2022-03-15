#include <iostream>
#include <thread>
#include <mutex>
#include <ctime>
#include <vector>
using namespace std;
std::mutex mtx;
size_t s_count = 0;

std::atomic<size_t> a_count(0);
void testfunc()
{
	for (int i = 0; i < 1e6; ++i)
	{
#if 0
		std::unique_lock<std::mutex> lock(mtx);
		s_count++;
#else
		a_count++;
#endif

	}
}

std::atomic_int test_a(0);
std::atomic_int test_b(0);


void testSeq()
{
	for (auto i = 0; i < 1e6; ++i)
	{
		test_a.store(i,std::memory_order_relaxed);	//使用弱序内存模型 增速10倍以上
		test_b.store(i,std::memory_order_relaxed);
	}
}

int main(int argc, char** argv)
{
	clock_t start_time = clock();

	vector<thread> threads;

	for (size_t i = 0; i < 10; ++i)
		//threads.emplace_back(thread(testfunc));
		threads.emplace_back(thread(testSeq));

	for (size_t i = 0; i < 10; ++i)
	{
		threads[i].join();
	}

	cout << "the number is " << test_a << endl;

	clock_t end_time = clock();

	cout << "cost time : " << end_time - start_time << "ms" << endl;
	//结果来看 原子操作的共享资源效率 提高了至少一倍
	return 0;
}