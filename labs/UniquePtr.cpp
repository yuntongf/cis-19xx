#include <utility>

template <typename T>
class UniquePtr {
public:
    UniquePtr(T* ptr = nullptr) : ptr_{ptr} {}

    UniquePtr(const UniquePtr& other) = delete;
    UniquePtr& operator=(const UniquePtr& other) = delete;

    UniquePtr(UniquePtr&& other) noexcept
    : ptr_{other.ptr_} {
        other.ptr_ = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) {
        if (this != other) {
            reset(std::exchange(other.ptr_, nullptr));
        }
        return *this;
    }

    ~UniquePtr() {
        delete ptr_;
    }

    void reset(T* new_ptr = nullptr) noexcept {
        delete ptr_;
        ptr_ = new_ptr;
    }

    T* get() const noexcept {
        return ptr_;
    }

    T* release() noexcept {
        return std::exchange(ptr_, nullptr);
    }

    T& operator*() const {
        return *ptr_;
    }

    T* operator->() const {
        return ptr_;
    }
private:
    T* ptr_;
};

int main() {

}