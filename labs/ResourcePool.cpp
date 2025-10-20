#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <cassert>
#include <tuple>

class ClientSession {
public:
    ClientSession(int client_id) : id_{client_id} {}
private:
    int id_;
};

class SessionPool {
public:
    static SessionPool& GetInstance() {
        static auto session = SessionPool{10, []{return new ClientSession(10);}};
        return session;
    }

    std::shared_ptr<ClientSession> Acquire() {
        std::unique_lock<std::mutex> lck{mtx_};
        cv_.wait(lck, [this]{return !sessions_.empty() || created_ < cap_;});
        if (sessions_.empty() && created_ < cap_) {
            sessions_.emplace(factory_());
            ++created_;
        }
        auto session_ptr = std::move(sessions_.front());
        sessions_.pop();
        return {session_ptr.release(), [this](ClientSession* ptr){
            sessions_.emplace(ptr);
            cv_.notify_one();
        }};
    }
private:
    SessionPool(std::size_t size, std::function<ClientSession*()> factory) : factory_{factory}, cap_{size}, created_{0}{}

private:
    using SessionPtr = std::shared_ptr<SessionPool>;
    std::function<ClientSession*()> factory_;
    std::size_t cap_;
    std::size_t created_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::queue<std::unique_ptr<ClientSession>> sessions_;
};

class ThreadPool {
public:
    ThreadPool(int num_threads) {
        for (int i = 0; i < num_threads; ++i) {
            workers_.emplace_back([this](){
                while (true) {
                    thread_local std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lck(mtx_);
                        cv_.wait(lck, [this]{return !jobs_.empty() || stopped;});
                        if (stopped) {
                            break;
                        }
                        task.swap(jobs_.front());
                        jobs_.pop();
                    }
                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lck(mtx_);
            stopped = true;
        }
        cv_.notify_all();
        for (auto& t : workers_) {
            t.join();
        }
    }

    template <typename F, typename... Args>
    auto Submit(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>> {
        using ReturnType = std::invoke_result_t<F, Args...>;
        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
            [
                f = std::forward<F>(f),
                tup = std::make_tuple(std::forward<Args>(args)...)
            ]() mutable {
                return std::apply(std::move(f), std::move(tup));
            }
        );
        auto fut = task->get_future();
        {
            std::unique_lock<std::mutex> lck(mtx_);
            jobs_.push([task]{(*task)();});
        }
        cv_.notify_one();
        return fut;
    }
private:
    std::mutex mtx_;
    std::condition_variable cv_;
    std::queue<std::function<void()>> jobs_;
    std::vector<std::thread> workers_;
    bool stopped = false;
};

class ThreadPoolSingleton {
public:
    static ThreadPool& GetInstance() {
        static ThreadPool pool = ThreadPool(NUM_THREADS);
        return pool;
    }

    // do I need to worry about 
private:
    inline static constexpr int NUM_THREADS = 10;
    ThreadPoolSingleton();
    ~ThreadPoolSingleton();
};

int main() {

}