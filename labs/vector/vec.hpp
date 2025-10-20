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

    // rule of five

    Vec(std::size_t size = 0);
    Vec(const Vec& other);
    Vec& operator=(const Vec& other);
    Vec(Vec&& other) noexcept;
    Vec& operator=(Vec&& other) noexcept;
    ~Vec();

    // modifiers 

    void Reserve(std::size_t new_cap);
    void Resize(std::size_t new_used);
    void Swap(Vec& other) noexcept;
    void PushBack(const T& t);
    void Clear() noexcept;

    template <typename... Args>
    void Emplace(const_iterator it, Args&&... args);

    // attributes

    [[nodiscard]] bool Empty() const noexcept {
        return used_ == 0;
    }

    [[nodiscard]] std::size_t Size() const noexcept {
        return used_;
    }

    [[nodiscard]] std::size_t Capacity() const noexcept {
        return capacity_;
    }

    // getters and setters

    T& operator[](std::size_t idx) noexcept { return data_[idx]; }
    const T& operator[](std::size_t idx) const noexcept { return data_[idx]; }

    T& At(std::size_t idx) {
        if (idx >= used_) {
            throw std::out_of_range("Vec::at: index out of range");
        }
        return data_[idx];
    }

    const T& At(std::size_t idx) const {
        if (idx >= used_) {
            throw std::out_of_range("Vec::at: index out of range");
        }
        return data_[idx];
    }

    T& Front() noexcept { 
        return data_[0]; 
    }

    const T& Front() const noexcept { 
        return data_[0]; 
    }

    T& Back() noexcept { 
        return data_[used_ - 1]; 
    }

    const T& Back() const noexcept { 
        return data_[used_ - 1]; 
    }

    iterator Begin() noexcept { 
        return data_; 
    }

    const_iterator Begin() const noexcept { 
        return data_; 
    }

    const_iterator Cbegin() const noexcept { 
        return data_; 
    }

    iterator End() noexcept { 
        return data_ + used_; 
    }

    const_iterator End() const noexcept { 
        return data_ + used_; 
    }

    const_iterator Cend() const noexcept { 
        return data_ + used_; 
    }

private:
    void copyDataFrom(const Vec& other);

    void shiftBack(const_iterator it, std::size_t n);
    
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
      used_{size},
      capacity_{size}
{
    for (std::size_t i = 0; i < used_; ++i)
        alloc_traits::construct(alloc_, data_ + i);
}

template <typename T, typename Alloc>
Vec<T, Alloc>::Vec(const Vec& other)
    : alloc_{alloc_traits::select_on_container_copy_construction(other.alloc_)},
      data_{alloc_traits::allocate(alloc_, other.capacity_)},
      used_{other.used_},
      capacity_{other.capacity_}
{
    std::uninitialized_copy_n(other.data_, used_, data_);
}

template <typename T, typename Alloc>
auto Vec<T, Alloc>::operator=(const Vec& other) -> Vec& {
    if (this != &other) {
        if constexpr (alloc_traits::propagate_on_container_copy_assignment::value) {
            if (alloc_ != other.alloc_) {
                for (std::size_t i = 0; i < used_; ++i) {
                    alloc_traits::destroy(alloc_, data_ + i);
                }
                alloc_traits::deallocate(alloc_, data_, capacity_);
                alloc_ = other.alloc_;
            }
        }
        Vec tmp(other);
        Swap(tmp);
    }
    return *this;
}

template <typename T, typename Alloc>
Vec<T, Alloc>::Vec(Vec&& other) noexcept
    : alloc_{std::move(other.alloc_)},
      data_{other.data_},
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
        Clear();
        if constexpr (alloc_traits::propagate_on_container_move_assignment::value) {
            alloc_ = std::move(other.alloc_);
        }
        data_ = other.data_;
        used_ = other.used_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.used_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

template <typename T, typename Alloc>
Vec<T, Alloc>::~Vec() {
    Clear();
    if (data_) {
        alloc_traits::deallocate(alloc_, data_, capacity_);
    }
}

template <typename T, typename Alloc>
void Vec<T, Alloc>::Reserve(std::size_t new_cap) {
    if (new_cap <= capacity_) {
        return;
    }

    T* new_data = alloc_traits::allocate(alloc_, new_cap);

    if constexpr (std::is_nothrow_move_constructible_v<T>)
        std::uninitialized_move_n(data_, used_, new_data);
    else
        std::uninitialized_copy_n(data_, used_, new_data);

    for (std::size_t i = 0; i < used_; ++i) {
        alloc_traits::destroy(alloc_, data_ + i);
    }

    if (data_) {
        alloc_traits::deallocate(alloc_, data_, capacity_);
    }

    data_ = new_data;
    capacity_ = new_cap;
}

template <typename T, typename Alloc>
void Vec<T, Alloc>::Resize(std::size_t new_used) {
    if (new_used < used_) {
        while (used_ > new_used) {
            alloc_traits::destroy(alloc_, data_ + --used_);
        }
    } else {
        Reserve(new_used);
        for (; used_ < new_used; ++used_) {
            alloc_traits::construct(alloc_, data_ + used_);
        }
    }
}

template <typename T, typename Alloc>
void Vec<T, Alloc>::PushBack(const T& value) {
    if (used_ == capacity_) {
        Reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    alloc_traits::construct(alloc_, data_ + used_, value);
    ++used_;
}

template <typename T, typename Alloc>
template <typename... Args>
void Vec<T, Alloc>::Emplace(const_iterator it, Args&&... args) {
    std::size_t pos = it - data_;
    if (used_ == capacity_) {
        Reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    // shift right by 1 from the end
    for (std::size_t i = used_; i > pos; --i) {
        alloc_traits::construct(alloc_, data_ + i, std::move_if_noexcept(data_[i - 1]));
        alloc_traits::destroy(alloc_, data_ + i - 1);
    }

    alloc_traits::construct(alloc_, data_ + pos, std::forward<Args>(args)...);
    ++used_;
}

template <typename T, typename Alloc>
void Vec<T, Alloc>::Swap(Vec& other) noexcept {
    std::swap(alloc_, other.alloc_);
    std::swap(data_, other.data_);
    std::swap(used_, other.used_);
    std::swap(capacity_, other.capacity_);
}

template <typename T, typename Alloc>
void Vec<T, Alloc>::copyDataFrom(const Vec& other) {
    std::uninitialized_copy_n(other.data_, other.used_, data_);
    used_ = other.used_;
}

template <typename T, typename Alloc>
void Vec<T, Alloc>::shiftBack(const_iterator it, std::size_t n) {
    std::size_t pos = it - data_;
    if (used_ + n > capacity_)
        Reserve((used_ + n) * 2);

    for (std::size_t i = used_; i > pos; --i) {
        alloc_traits::construct(alloc_, data_ + i + n - 1, std::move_if_noexcept(data_[i - 1]));
        alloc_traits::destroy(alloc_, data_ + i - 1);
    }
    used_ += n;
}

template <typename T, typename Alloc>
void Vec<T, Alloc>::Clear() noexcept {
    while (used_ > 0) {
        alloc_traits::destroy(alloc_, data_ + --used_);
    }
}
