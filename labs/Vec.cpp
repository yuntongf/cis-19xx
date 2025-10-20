#include <iostream>
#include <cstddef>
#include <memory>

template <typename T, typename Alloc = std::allocator<T>>
class Vec {
public:
    using alloc_traits = std::allocator_traits<Alloc>;
    using value_type = T;
    using iterator = T*;
    using reverse_iterator = T*;
    using const_iterator = const T*;
    using const_reverse_iterator = const T*;

    Vec(std::size_t size = 10);

    Vec(const Vec& other);

    Vec& operator=(const Vec& other);

    Vec(Vec&& other) noexcept;

    Vec& operator=(Vec&& other) noexcept;

    ~Vec();

    void reserve(std::size_t new_cap);

    void resize(std::size_t new_used);

    void swap(Vec& other) noexcept;

    void push_back(const T& t);

    template <typename... Args>
    void emplace(const_iterator it, Args&&... args);

    [[nodiscard]] iterator begin() {
        return data_;
    }

    [[nodiscard]] iterator end() {
        return data_ + used_;
    }

    [[nodiscard]] const_iterator begin() const {
        return data_;
    }

    [[nodiscard]] const_iterator end() const {
        return data_ + used_;
    }

    [[nodiscard]] reverse_iterator rbegin() {
        return data_ + used_ - 1;
    }

    [[nodiscard]] reverse_iterator rend() {
        return data_ - 1;
    }

    [[nodiscard]] const_reverse_iterator rbegin() const {
        return data_ + used_ - 1;
    }

    [[nodiscard]] const_reverse_iterator rend() const {
        return data_ - 1;
    }

    [[nodiscard]] bool empty() const noexcept {
        return used_ == 0;
    }

    [[noidscard]] std::size_t size() const noexcept {
        return used_;
    }

    [[nodiscard]] std::size_t capacity() const noexcept {
        return capacity_;
    }
private:
    void copy_data_from(const Vec& other);

    void shift_back(const_iterator it, std::size_t n);
    
private:
    Alloc alloc_;
    T* data_;
    std::size_t used_;
    std::size_t capacity_;
};

template <typename T, typename Alloc>
Vec<T, Alloc>::Vec(std::size_t size)
: alloc_{},
  data_{size == 0 ? nullptr : alloc_traits::allocate(alloc_, size)},
  used_{0},
  capacity_{size}
{
    for (int i = 0; i < size; ++i) {
        alloc_traits::construct(alloc_, data_ + i);
        ++used_;
    }
}

template <typename T, typename Alloc>
Vec<T, Alloc>::Vec(const Vec& other)
: alloc_{other.alloc_},
  data_{alloc_traits::allocate(alloc_, other.capacity_)},
  used_{0},
  capacity_{other.capacity_}
{
    // copy_data_from(other);
    std::copy(other.begin(), other.end(), data_);
}

template <typename T, typename Alloc>
auto Vec<T, Alloc>::operator=(const Vec& other) -> Vec& {
    if (this != &other) {
        Vec temp(other);
        swap(temp);
    }
    return *this;
}

template <typename T, typename Alloc>
Vec<T, Alloc>::Vec(Vec&& other) noexcept
: alloc_{std::move(other,alloc_)},
  data_{std::move(other.data_)},
  used_{other.used_},
  capacity_{other.capacity_}
{
    other.data_ = nullptr;
    other.used_ = 0;
    other.capacity_ = 0;
}

template <typename T, typename Alloc>
auto Vec<T, Alloc>::operator=(Vec&& other) noexcept -> Vec& {
    if (this != &other) {
        Vec temp = std::move(other);
        swap(temp);
    }
    return *this;
}

template <typename T, typename Alloc>
void Vec<T, Alloc>::swap(Vec& other) noexcept {
    std::swap(alloc_, other.alloc_);
    std::swap(data_, other.data_);
    std::swap(used_, other.used_);
    std::swap(capacity_, other.capacity_);
}

template <typename T, typename Alloc>
Vec<T, Alloc>::~Vec() {
    int used_copy = used_;
    for (int i = 0; i < used_copy; ++i) {
        alloc_traits::destroy(alloc_, data_ + i);
        --used_;
    }
    if (data_) {
        alloc_traits::deallocate(alloc_, data_, capacity_);
    }
}

template <typename T, typename Alloc>
void Vec<T, Alloc>::reserve(std::size_t new_cap) {
    if (capacity_ >= new_cap) {
        return;
    }
    Vec temp(new_cap);
    for (int i = 0; i < used_; ++i) {
        alloc_traits::construct(temp.alloc_, temp.data_ + i, std::move(data_[i]));
        ++temp.used_;
    }
    // std::uninitialized_move(begin(), end(), temp.begin());
    swap(temp);
}

template <typename T, typename Alloc>
void Vec<T, Alloc>::copy_data_from(const Vec& other) {
    for (int i = 0; i < other.used_; ++i) {
        alloc_traits::construct(alloc_, data_ + i, other.data_[i]);
        ++used_;
    }
}

template <typename T, typename Alloc>
void Vec<T, Alloc>::resize(std::size_t new_used) {
    if (new_used <= used_) {
        while (used_ > new_used) {
            alloc_traits::destroy(alloc_, data_ + used_ - 1);
            --used_;
        }
    } else {
        reserve(new_used);
        for (int i = used_; i < new_used; ++i) {
            alloc_traits::construct(alloc_, data_ + i);
            ++used_;
        }
    }
}

template <typename T, typename Alloc>
void Vec<T, Alloc>::push_back(const T& t) {
    if (used_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    alloc_traits::construct(alloc_, data_ + used_, t);
    ++used_;
}

template <typename T, typename Alloc>
template <typename... Args>
void Vec<T, Alloc>::emplace(const_iterator it, Args&&... args) {
    shift_back(it, 1);
    alloc_traits::construct(alloc_, it, std::forward<Args>(args)...);
}

template <typename T, typename Alloc>
void Vec<T, Alloc>::shift_back(const_iterator cit, std::size_t n) {
    if (used_ + n > capacity_) {
        reserve(used_ + n);
    }
    // std::memmove(it + n * sizeof(T), it, n * sizeof(T)); // can I use this here?
    // shift from back
    for (int i = rbegin() - begin(); i >= cit - begin(); --i) {
        alloc_traits::construct(alloc_, data_ + i + n, std::move(data_[i]));
        alloc_traits::destroy(alloc_, data_ + i);
    }
}

int main() {
    return 0;
}