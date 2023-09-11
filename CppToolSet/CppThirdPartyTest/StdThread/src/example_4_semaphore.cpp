#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <stdio.h>
#include <semaphore>	// cpp20

#include "example_4_semaphore.h"

namespace Example_4
{

	class SemaphoreCpp11
	{
	public:
		SemaphoreCpp11(long count = 0)
			: _count(count)
		{
		}

		void acquire()
		{
			std::unique_lock<std::mutex> lock { _mtx };
			_condVar.wait(lock, [&]() {return _count > 0; });
			_count--;
		}

		void release(long count)
		{
			while (count--)
				release();
		}

		void release()
		{
			std::unique_lock<std::mutex>lock { _mtx };
			_count++;
			_condVar.notify_one();
		}

	private:
		std::mutex _mtx;
		std::condition_variable _condVar;
		long _count;
	};

	void TestSemaphoreCpp11()
	{
		printf("TestSemaphoreCpp11: \n");

		SemaphoreCpp11 _sem(0);

		auto threadFunc = [&]() {
			printf("Thread Begin! \n");
			_sem.acquire();
			printf("Thread Finish! \n");
		};

		std::thread thread1 { threadFunc };
		std::thread thread2 { threadFunc };
		std::thread thread3 { threadFunc };
		std::thread thread4 { threadFunc };
		std::thread thread5 { threadFunc };

		_sem.release(3);

		std::this_thread::sleep_for(std::chrono::seconds(3));

		_sem.release(2);

		thread1.join();
		thread2.join();
		thread3.join();
		thread4.join();
		thread5.join();

		printf("\n\n");
	}

	void TestSemaphoreCpp20()
	{
		printf("TestSemaphoreCpp20: \n");

		std::counting_semaphore<5> _sem(0);

		auto threadFunc = [&]() {
			printf("Thread Begin! \n");
			_sem.acquire();
			printf("Thread Finish! \n");
		};

		std::thread thread1 { threadFunc };
		std::thread thread2 { threadFunc };
		std::thread thread3 { threadFunc };
		std::thread thread4 { threadFunc };
		std::thread thread5 { threadFunc };

		_sem.release(3);

		std::this_thread::sleep_for(std::chrono::seconds(3));

		_sem.release(2);

		thread1.join();
		thread2.join();
		thread3.join();
		thread4.join();
		thread5.join();

		printf("\n\n");
	}

	void Run()
	{
		TestSemaphoreCpp11();
		TestSemaphoreCpp20();
	}
}
