#pragma once

#include <functional>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <future>
#include <utility>

class ThreadPool {
public:
    ThreadPool(int capacity) : stopped_{false} {
        for (int i = 0; i < capacity; ++i) {
            workers_.emplace_back([this](){
                while (true) {
                    Task task;
                    {
                        std::unique_lock<std::mutex> lck(mtx_);
                        cv_.wait(lck, [this]{return !tasks_.empty() || stopped_;});
                        if (stopped_) {
                            break;
                        }
                        task = tasks_.front();
                        tasks_.pop();
                    }
                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        Stop();
        for (auto& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    template <typename F, typename... Args>
    auto Submit(F&& f, Args&&... args) {
        using Result = std::invoke_result_t<F, Args...>;
        auto task = std::make_shared<std::packaged_task<Result()>>(
            [f = std::forward<F>(f), tup = std::make_tuple(std::forward<Args>(args)...)](){
                return std::apply(f, tup);
            }
        );
        auto fut = task->get_future();
        {
            std::scoped_lock lck(mtx_);
            tasks_.push([task]{(*task)();});
        }
        cv_.notify_one();
        return fut;
    }

    void Stop() {
        {
            std::scoped_lock lck(mtx_);
            stopped_ = true;
        }
        cv_.notify_all();
    }

private:
    bool stopped_;
    using Task = std::function<void()>;
    std::queue<Task> tasks_;
    std::vector<std::thread> workers_;
    std::condition_variable cv_;
    std::mutex mtx_;
};