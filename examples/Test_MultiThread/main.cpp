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

void test_atomic()
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
}


void callback()
{
	std::cout << "function finish!!!" << std::endl;
}

#include <functional>
#include <future>
void dosomething(std::function<void()> func)
{
	std::cout << "hello world" << std::endl;

	func();
}

class Te
{
public:
	void callback()
	{
		std::cout << "member callback ." << std::endl;
	}
};


void testcallback()
{
	//package 可以理解为一个函数包   
	std::packaged_task<void()> foo(callback);
	auto ft = foo.get_future();
	//可以直接与thread绑定
	std::thread(std::move(foo)).detach();
	std::cout << "!" << " " << ft.valid() << std::endl;
	ft.get();
	//可以判断有效性判断线程是否执行完毕(前提是线程能正常执行完毕)
	std::cout << "!" << " " << ft.valid() << std::endl;
	
	static Te te;
	std::thread  th(dosomething, std::bind(&Te::callback ,&te));
	th.detach();


	std::promise<int>  prm;
	auto print_promise = [&prm]()
	{
		auto fut = prm.get_future();
		std::cout << "value is : " << fut.get() << std::endl;
	};

	thread thr(print_promise);
	//set_value 只能执行一次
	prm.set_value(10);

	thr.join();
}
#include <chrono>
int main(int argc, char** argv)
{
	//test_atomic();

	testcallback();

	std::this_thread::sleep_for(std::chrono::seconds(1));
	return 0;
}