#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <type_traits>

namespace Util
{
    class ThreadPool
    {
    public:
        inline ThreadPool(size_t threadsNum): _stop(false)
        {
            for (size_t i = 0; i < threadsNum; ++i)
            {
                _workers.emplace_back([this]
                {
                    while (true)
                    {
                        std::function<void()> task;

                        {
                            std::unique_lock<std::mutex> lock(_queueMutex);

                            _condition.wait(lock,[this]
                            {
                                return _stop || !_tasks.empty();
                            });

                            if (_stop && _tasks.empty())
                                return;

                            task = std::move(_tasks.front());

                            _tasks.pop();
                        }

                        task();
                    }
                }
                );
            }
        }

        ~ThreadPool()
        {
            {
                std::unique_lock<std::mutex> lock(_queueMutex);
                _stop = true;
            }

            _condition.notify_all();

            for (std::thread& worker: _workers)
                worker.join();
        }

        template<class F, class... Args>
        std::future<typename std::invoke_result_t<F(Args...)>::type> Enqueue(F&& f, Args&&... args)
        {
            using ReturnType = typename std::invoke_result_t<F(Args...)>::type;

            auto task = std::make_shared< std::packaged_task<ReturnType()> >(
                    std::bind(std::forward<F>(f), std::forward<Args>(args)...)
            );

            std::future<ReturnType> res = task->get_future();

            {
                std::unique_lock<std::mutex> lock(_queueMutex);

                if(_stop)
                    return;

                _tasks.emplace([task](){ (*task)(); });
            }

            _condition.notify_one();
            return res;
        }

    private:
        std::vector< std::thread > _workers;
        std::queue< std::function<void()> > _tasks;

        std::mutex _queueMutex;
        std::condition_variable _condition;
        bool _stop;
    };

}