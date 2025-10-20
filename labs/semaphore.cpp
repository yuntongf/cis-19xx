#include <atomic>
#include <thread>
#include <iostream>
#include <vector>
#include <chrono>

class Semaphore {
public:
    Semaphore(int count = 1) : count_(count) {}

    void acquire() {
        while (true) {
            int count = count_.load(std::memory_order_relaxed);
            if (count == 0) {
                std::this_thread::yield();
                continue;
            }
            if (count_.compare_exchange_strong(count, count - 1, std::memory_order_acq_rel)) {
                break;
            }
        }
    }

    void release() {
        count_.fetch_add(1, std::memory_order_acq_rel);
    }
private:
    std::atomic<int> count_;
};

int main() {
    Semaphore sem(3);
    std::atomic<int> counter = 0;
    std::vector<std::thread> threads;

    auto worker = [&](int id) {
        sem.acquire();
        std::cout << "Thread " << id << " entered\n";
        int current = ++counter;
        if (current > 3) {
            std::cerr << "Error: too many threads entered!\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        --counter;
        std::cout << "Thread " << id << " leaving\n";
        sem.release();
    };

    // Launch 10 threads but only 3 should run concurrently
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(worker, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "All threads finished.\n";

    // Test with 0-count semaphore (should block until released)
    Semaphore zero_sem(0);
    bool passed = false;
    std::thread t1([&] {
        zero_sem.acquire();
        passed = true;
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if (!passed) std::cout << "Acquire() correctly blocked\n";
    zero_sem.release();
    t1.join();
    std::cout << "Acquire() unblocked after release\n";

    return 0;
}
