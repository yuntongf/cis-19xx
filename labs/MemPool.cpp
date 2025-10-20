#include <cstddef>
#include <vector>
#include <memory>

template <typename T>
class MemPool {
public:
    MemPool(std::size_t num_elements)
    : data_{static_cast<T*>(::operator new(num_elements * sizeof(T), std::align_val_t(alignof(T))))},
      is_used_(num_elements, false),
      capacity_{num_elements}
    {
        free_list_.reserve(num_elements);
        for (int i = 0; i < num_elements; ++i) {
            free_list_.push_back(i);
        }
    }

    MemPool(const MemPool& other) = delete;

    MemPool& operator=(const MemPool& other) = delete;

    ~MemPool() {
        for (int i = 0; i < capacity_; ++i) {
            if (is_used_[i]) {
                data_[i].~T();
            }
        }
        ::operator delete(data_, std::align_val_t(alignof(T)));
    }

    std::unique_ptr<T> acquire() {
        if (free_list_.empty()) {
            return nullptr;
        }
        auto idx = free_list_.back();
        new (data_ + idx) T(); // we do this first since if it throws we are still in consistent state

        free_list_.pop_back();
        is_used_[idx] = true;

        return std::unique_ptr<T>(data_ + idx, [this](T* t){
            release(t);
        });
    }

    void release(T* ptr) {
        ptr->~T();
        auto idx = ptr - data_;
        is_used_[idx] = false;
        free_list_.push_back(idx);
    }

private:
    T* data_;
    std::vector<std::size_t> free_list_;
    std::vector<bool> is_used_;
    std::size_t capacity_;
};

int main() {
    return 0;
}