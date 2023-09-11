#include <stdio.h>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>

#include "example_3_condition_variable.h"

namespace Example_3
{
    std::condition_variable condVar;
    std::mutex mtx;
    std::queue<int> dataQueue;
    bool stopped = false;

    void ProducerThread()
    {
        std::size_t count = 100;
        while (count--)
        {
            // Always lock before changing state guarded by a mutex and condition_variable
            std::lock_guard<std::mutex> lockGuard { mtx };
            dataQueue.push(count);

            std::cout << "Producer Data: " << count << std::endl;

            condVar.notify_one();
        }

        // All done. Acquire the lock, set the stopped flag,
        std::lock_guard<std::mutex> lockGuard { mtx };

        std::cout << "Producer is done!" << std::endl;

        stopped = true;
        condVar.notify_one();
    }

    void ConsumerThread()
    {
        while (true) 
        {
            std::unique_lock<std::mutex> uniqueLock { mtx };

            condVar.wait(uniqueLock, [&]()
                {
                    return stopped || !dataQueue.empty();
                });

            while (!dataQueue.empty())
            {
                const auto val = dataQueue.front();
                dataQueue.pop();

                std::cout << "Consumer popped: " << val << std::endl;
            }

            if (stopped) 
            {
                std::cout << "Consumer is done!" << std::endl;
                break;
            }

        }
    }

	void Run()
    {
        
        std::thread producer(ProducerThread);
        std::thread consumer(ConsumerThread);

        consumer.join();
        producer.join();

        std::cout << "Example Completed!" << std::endl;
	}
}