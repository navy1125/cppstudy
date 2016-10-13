#include <iostream>
#include <functional>
#include <thread>
#include <future>

using std::cout;
using std::endl;

class thread_c
{
    public:
	int thread_fun(int num)
	{
		cout << "calculating....." << endl;
		int count = num;
		for (int i = 0; i < num; i++)
		{
			count++;
		}
		cout << "end calculate" << endl;
		return count;
	}
};

int main()
{
	thread_c thread_obj;
	std::future< int > ret = std::async(std::bind(&thread_c::thread_fun, &thread_obj, 10000));
	cout << "async get value = " << ret.get() << endl;

	return 0;
}
