#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <utility>

template <typename T>
class ResourcePool {
public:
    using ResourcePtr = std::unique_ptr<T>;
    using Creator = std::function<ResourcePtr()>;

    ResourcePool(std::size_t capacity, Creator creator) 
    : capacity_{capacity}, 
      size_{0}, 
      creator_(creator) {}

    ResourcePtr acquire() {
        std::unique_lock<std::mutex> lck(mtx_);
        cv_.wait(lck, [this]{return !avail_.empty() || size_ < capacity_;});
        if (avail_.empty() && size_ < capacity_) {
            avail_.push_back(std::move(creator_()));
            ++size_;
        }
        auto r = std::move(avail_.back());
        avail_.pop_back();
        return std::move(r);
    }

    void release(ResourcePtr&& ptr) {
        std::scoped_lock<std::mutex> lck(mtx_);
        avail_.push_back(std::move(ptr));
        cv_.notify_one();
    }
private:
    std::size_t capacity_;
    std::size_t size_;
    std::vector<ResourcePtr> avail_;
    std::mutex mtx_;
    std::condition_variable cv_;
    Creator creator_;
};

class Connection {
public:
    static std::size_t id;

    Connection() : id_{id++} {
        std::cout << "Inititializing connection with id " << id_ << '\n';
    }

    void send(const char* str) const {
        std::cout << "Connection [" << id_ << "] sending: " << str;
    }

    [[nodiscard]] std::size_t getId() const noexcept {
        return id_;
    }
private:
    std::size_t id_;
};

std::size_t Connection::id = 0;

using ConnectionPool = ResourcePool<Connection>;
    
struct ConnectionDeleter {
    std::weak_ptr<ConnectionPool> pool;

    void operator()(Connection* ptr) const {
        if (auto p = pool.lock()) {
            std::cout << "Releasing connection with id " << ptr->getId() << '\n';
            p->release(std::unique_ptr<Connection>(ptr));
        } else {
            std::cerr << "Resource pool no longer exists. Leaking connection with id " << ptr->getId() << '\n';
            delete ptr;
        }
    }
};

std::unique_ptr<Connection, ConnectionDeleter> make_managed_connection(std::shared_ptr<ConnectionPool> pool) {
    auto conn = pool->acquire();
    auto raw = conn.release();
    ConnectionDeleter deletor{pool};
    return {raw, deletor};
}

int main() {
    auto pool = std::make_shared<ConnectionPool>(10, [](){
        return std::make_unique<Connection>();
    });

    auto conn1 = make_managed_connection(pool);
    conn1->send("Hi!\n");
}