#include <stdio.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

#include "example_2_atomic_mutex.h"

namespace Example_2
{
	void MutexThreadJob(int& targetNumber, std::mutex& mutex)
	{
		for (int i = 0; i < 1000; i++)
		{
			mutex.lock();
			targetNumber++;
			mutex.unlock();
		}
	}

	void LockGuardThreadJob(int& targetNumber, std::mutex& mutex)
	{
		for (int i = 0; i < 1000; i++)
		{
			std::lock_guard<std::mutex> guard(mutex);
			targetNumber++;
		}
	}

	void AtomicThreadJob(std::atomic<int>& targetNumber)
	{
		for (int i = 0; i < 1000; i++)
			targetNumber++;
	}

	void MutexTest()
	{
		int testNumber = 0;
		std::mutex mutex;

		auto timeStart = std::chrono::steady_clock::now();

		std::thread thread1(MutexThreadJob, std::ref(testNumber), std::ref(mutex));
		std::thread thread2(MutexThreadJob, std::ref(testNumber), std::ref(mutex));

		thread1.join();
		thread2.join();

		auto timeEnd = std::chrono::steady_clock::now();

		auto timeDuration = std::chrono::duration_cast<std::chrono::microseconds>(timeEnd - timeStart);
		long long mutexTestTime = timeDuration.count();

		printf("Mutex Test Final Value: %d, with %lld microseconds \n", testNumber, mutexTestTime);
	}

	void LockGuardTest()
	{
		int testNumber = 0;
		std::mutex mutex;

		auto timeStart = std::chrono::steady_clock::now();

		std::thread thread1(LockGuardThreadJob, std::ref(testNumber), std::ref(mutex));
		std::thread thread2(LockGuardThreadJob, std::ref(testNumber), std::ref(mutex));

		thread1.join();
		thread2.join();

		auto timeEnd = std::chrono::steady_clock::now();

		auto timeDuration = std::chrono::duration_cast<std::chrono::microseconds>(timeEnd - timeStart);
		long long lockGuardTestTime = timeDuration.count();

		printf("Lockguard Test Final Value: %d, with %lld microseconds \n", testNumber, lockGuardTestTime);
	}

	void AtomicTest()
	{
		std::atomic<int> testNumber = 0;

		auto timeStart = std::chrono::steady_clock::now();

		std::thread thread1(AtomicThreadJob, std::ref(testNumber));
		std::thread thread2(AtomicThreadJob, std::ref(testNumber));

		thread1.join();
		thread2.join();

		auto timeEnd = std::chrono::steady_clock::now();

		auto timeDuration = std::chrono::duration_cast<std::chrono::microseconds>(timeEnd - timeStart);
		long long atomicTestTime = timeDuration.count();

		printf("Atomic Test Final Value: %d, with %lld microseconds \n", testNumber.load(), atomicTestTime);
	}

	void Run()
	{
		MutexTest();
		LockGuardTest();
		AtomicTest();
	}
}

