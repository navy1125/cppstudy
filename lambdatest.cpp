/*
 * sched_thread.c
 * Demonstrate dynamic scheduling policy use.
 */
#include <thread>    // std::thread
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <vector>
#include <iostream>  // std::cout
int callback(int i)
{
	return i;
}
using namespace std;
class CTest
{
	public:
		CTest() : m_nData(20) { NULL; }
		void TestLambda4()
		{
			std::vector<int> vctTemp;
			for(int i = 1 ; i < 1000 ;i ++)
			{
				vctTemp.push_back(i);
			}


			int a = 10;
			for(int i = 1 ; i < 1000000 ;i ++)
			{
				for(auto &it: vctTemp)
				{
					a = callback(a + it);
				}
			}
			cout <<"TestLambda4 end" << endl;
		}
		void TestLambda3()
		{
			std::vector<int> vctTemp;
			for(int i = 1 ; i < 1000 ;i ++)
			{
				vctTemp.push_back(i);
			}


			int a = 10;
			for(int i = 1 ; i < 1000000 ;i ++)
			{
				for(auto it: vctTemp)
				{
					a = callback(a + it);
				}
			}
			cout <<"TestLambda3 end" << endl;
		}
		void TestLambda2()
		{
			std::vector<int> vctTemp;
			for(int i = 1 ; i < 1000 ;i ++)
			{
				vctTemp.push_back(i);
			}


			int a = 10;
			for(int i = 1 ; i < 1000000 ;i ++)
			{
				for(auto it: vctTemp)
				{
					a+=it;
				}
			}
			cout <<"TestLambda2 end" << endl;
		}
		void TestLambda1()
		{
			std::vector<int> vctTemp;
			for(int i = 1 ; i < 1000 ;i ++)
			{
				vctTemp.push_back(i);
			}


			int a = 10;
			for(int i = 1 ; i < 1000000 ;i ++)
			{
				for_each(vctTemp.begin(), vctTemp.end(), [&](int v){ a+=v; });
			}
			cout <<"TestLambda1 end" << endl;
		}
		void TestLambda()
		{
			std::vector<int> vctTemp;
			vctTemp.push_back(1);
			vctTemp.push_back(2);

			// 无函数对象参数，输出：1 2
			{
				for_each(vctTemp.begin(), vctTemp.end(), [](int v){ cout << v << endl; });
			}

			// 以值方式传递作用域内所有可见的局部变量（包括this），输出：11 12
			{
				int a = 10;
				for_each(vctTemp.begin(), vctTemp.end(), [=](int v){ cout << v+a << endl; });
			}

			// 以引用方式传递作用域内所有可见的局部变量（包括this），输出：11 13 12
			{
				int a = 10;
				for_each(vctTemp.begin(), vctTemp.end(), [&](int v)mutable{ cout << v+a << endl; a++; });
				cout << a << endl;
			}

			// 以值方式传递局部变量a，输出：11 13 10
			{
				int a = 10;
				for_each(vctTemp.begin(), vctTemp.end(), [a](int v)mutable{ cout << v+a << endl; a++; });
				cout << a << endl;
			}

			// 以引用方式传递局部变量a，输出：11 13 12
			{
				int a = 10;
				for_each(vctTemp.begin(), vctTemp.end(), [&a](int v){ cout << v+a << endl; a++; });
				cout << a << endl;
			}

			// 传递this，输出：21 22
			{
				for_each(vctTemp.begin(), vctTemp.end(), [this](int v){ cout << v+m_nData << endl; });
			}

			// 除b按引用传递外，其他均按值传递，输出：11 12 17
			{
				int a = 10;
				int b = 15;
				for_each(vctTemp.begin(), vctTemp.end(), [=, &b](int v){ cout << v+a << endl; b++; });
				cout << b << endl;
			}


			// 操作符重载函数参数按引用传递，输出：2 3
			{
				for_each(vctTemp.begin(), vctTemp.end(), [](int &v){ v++; });
				for_each(vctTemp.begin(), vctTemp.end(), [](int v){ cout << v << endl; });
			}

			// 空的Lambda表达式
			{
				[](){}();
				[]{}();
			}
		}

	private:
		int m_nData;
};
void call4()
{
	CTest test;test.TestLambda4();
}
void call3()
{
	CTest test;test.TestLambda3();
}
void call1()
{
	CTest test;test.TestLambda1();
}
void call2()
{
	CTest test;test.TestLambda2();
}
int main(int argc, char *argv[])
{
	std::vector< std::thread > threads;
	threads.push_back(std::thread(call1));
	threads.push_back(std::thread(call2));
	threads.push_back(std::thread(call4));
	threads.push_back(std::thread(call3));
	for (auto& th : threads)
		th.join();
	
	//test.TestLambda1();
}
