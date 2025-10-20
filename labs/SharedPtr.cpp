#include <cstddef>
#include <memory>
#include <atomic>
#include <iostream>
#include <sstream>

struct CB {
    std::atomic<std::size_t> use_count;
    std::atomic<std::size_t> weak_count;
};

template <class T>
class WeakPtr;

template <class T>
class SharedPtr {
public:
    friend class WeakPtr<T>;

    explicit SharedPtr(T* t) : t_{t}, cb_(t ? new CB{1, 1} : nullptr) {}

    SharedPtr(const SharedPtr& other) : t_(other.t_), cb_(other.cb_) { // noexcept?
        increment_use();
    }

    SharedPtr(const WeakPtr<T>& wptr) {
        if (wptr.expired()) {
            throw std::bad_weak_ptr();
        }
        t_ = wptr.t_;
        cb_ = wptr.cb_;
        increment_use();
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            SharedPtr temp(other);
            swap(temp);
        }
        return *this;
    }

    SharedPtr(SharedPtr&& other) : t_{other.t_}, cb_{other.cb_} {
        other.cb_ = nullptr;
        other.t_ = nullptr;
    }

    SharedPtr& operator=(SharedPtr&& other) {
        if (this != &other) {
            SharedPtr temp(std::move(other));
            swap(temp);
        }
        return *this;
    }

    ~SharedPtr() {
        if (!cb_) {
            return;
        }
        decrement_use();
        if (cb_->use_count.load() == 0) {
            // is this thread safe??
            release_resource();
            --cb_->weak_count; // observers are now on their own
        }
        if (cb_->weak_count.load() == 0) {
            release_cb();
        }
    }

    T* operator->() {
        return t_;
    }

    T& operator*() {
        return *t_;
    }

    void swap(SharedPtr& other) noexcept {
        std::swap(t_, other.t_);
        std::swap(cb_, other.cb_);
    }

    void reset(T* t) {
        SharedPtr temp(t);
        swap(temp);
    }

    friend std::ostream& operator<<(std::ostream& os, const SharedPtr& p) {
        if (p.cb_) {
            os << "use count = " << p.cb_->use_count
            << ", weak count = " << p.cb_->weak_count
            << ", value = " << *p.t_;
        } else {
            os << "null SharedPtr";
        }
        return os;
    }
private:
    void increment_use() {
        if (cb_) {
            ++cb_->use_count;
        }
    }

    void decrement_use() {
        if (cb_) {
            if (auto curr_count = cb_->use_count.load()) {
                cb_->use_count.compare_exchange_strong(curr_count, curr_count - 1);
            }
        }
    }

    void release_resource() {
        std::cout << "Shared ptr releasing resource for object\n";
        delete t_;
    }

    void release_cb() {
        std::cout << "Shared ptr releasing resource for control block\n";
        delete cb_;
    }
private:
    T* t_;
    CB* cb_;
};

template <class T>
class WeakPtr {
public:
    friend class SharedPtr<T>;
    WeakPtr(const SharedPtr<T>& ptr) : cb_(ptr.cb_), t_{ptr.t_} {
        std::cout << "Constructing wptr from sptr\n";
        increment_weak();
    }

    WeakPtr(const WeakPtr& other) : cb_{other.cb_}, t_{other.t_} {
        increment_weak();
    }

    WeakPtr& operator=(const WeakPtr& other) {
        if (this != &other) {
            WeakPtr temp(other);
            swap(temp);
        }
        return *this;
    }

    WeakPtr(WeakPtr&& other) noexcept : cb_{other.cb_}, t_{other.t_} {
        other.cb_ = nullptr;
        other.t_ = nullptr;
    }

    WeakPtr& operator=(WeakPtr&& other) noexcept {
        if (this != &other) {
            WeakPtr temp(std::move(other));
            swap(temp);
        }
        return *this;
    }

    ~WeakPtr() {
        if (!cb_) {
            return;
        }
        decrement_weak();
        if (cb_->weak_count.load() == 0) {
            release_cb();
        }
    }

    std::size_t use_count() const {
        return cb_->use_count.load();
    }

    [[nodiscard]] bool expired() const {
        return use_count() == 0;
    }

    [[nodiscard]] SharedPtr<T> lock() {
        return SharedPtr(*this);
    }

    void swap(WeakPtr& other) noexcept {
        std::swap(cb_, other.cb_);
        std::swap(t_, other.t_);
    }

    friend std::ostream& operator<<(std::ostream& os, const WeakPtr& p) {
        if (p.cb_) {
            os << "use count = " << p.cb_->use_count
            << ", weak count = " << p.cb_->weak_count;
        } else {
            os << "null WeakPtr";
        }
        return os;
    }
private:
    void increment_weak() {
        ++cb_->weak_count;
    }

    void decrement_weak() {
        if (cb_->weak_count.load() > 0) {
            --cb_->weak_count;
        }
    }

    void release_cb() {
        std::cout << "Weak ptr releasing resource for control object\n";
        delete cb_;
    }
private:
    T* t_;
    CB* cb_;
};

int main() {
    SharedPtr<int> a(new int{5});
    SharedPtr<int> b(new int{8});
    std::cout << "a: " << a << "\n";
    std::cout << "b: " << b << "\n";
    b = a;
    std::cout << "a: " << a << "\n";
    std::cout << "b: " << b << "\n";
    WeakPtr<int> wa = a;
    a.reset(nullptr);
    std::cout << "reset a, wa: " << wa << "\n";
    std::cout << "a: " << a << "\n";
    std::cout << "b: " << b << "\n";
    b.reset(nullptr);
    std::cout << "a: " << a << "\n";
    std::cout << "b: " << b << "\n";
    std::cout << "reset b, wa: " << wa << "\n";
}