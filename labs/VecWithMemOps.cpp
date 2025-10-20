#include <iostream>
#include <cstddef>
#include <memory>
#include <cassert>
#include <string>
#include <iostream>

template <typename T, typename Alloc = std::allocator<T>>
class Vec {
public:
    using alloc_traits = std::allocator_traits<Alloc>;
    using value_type = T;
    using iterator = T*;
    using reverse_iterator = T*;
    using const_iterator = T const*;
    using const_reverse_iterator = T const*;

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

    [[nodiscard]] const_iterator cbegin() const {
        return data_;
    }

    [[nodiscard]] const_iterator cend() const {
        return data_ + used_;
    }

    [[nodiscard]] reverse_iterator rbegin() {
        return data_ + used_ - 1;
    }

    [[nodiscard]] reverse_iterator rend() {
        return data_ - 1;
    }

    [[nodiscard]] const_reverse_iterator rcbegin() const {
        return data_ + used_ - 1;
    }

    [[nodiscard]] const_reverse_iterator rcend() const {
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
    std::uninitialized_default_construct_n(data_, capacity_);
    used_ = size;
}

template <typename T, typename Alloc>
Vec<T, Alloc>::Vec(const Vec& other)
: alloc_{other.alloc_},
  data_{other.capacity_ == 0 ? nullptr : alloc_traits::allocate(alloc_, other.capacity_)},
  used_{0},
  capacity_{other.capacity_}
{   
    std::uninitialized_copy_n(other.data_, other.used_, data_);
    used_ = other.used_;
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
: alloc_{std::move(other.alloc_)},
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
    std::destroy_n(data_, used_);
    if (data_) {
        alloc_traits::deallocate(alloc_, data_, capacity_);
    }
}

template <typename T, typename Alloc>
void Vec<T, Alloc>::reserve(std::size_t new_cap) {
    if (capacity_ >= new_cap) {
        return;
    }

    T* new_data = alloc_traits::allocate(alloc_, new_cap);
    std::uninitialized_move(data_, data_ + used_, new_data);

    std::destroy_n(data_, used_);
    alloc_traits::deallocate(alloc_, data_, capacity_);

    data_ = new_data;
    capacity_ = new_cap;
}

template <typename T, typename Alloc>
void Vec<T, Alloc>::resize(std::size_t new_used) {
    if (new_used < used_) {
        std::destroy(data_ + new_used, end());
    } else {
        reserve(new_used);
        std::uninitialized_default_construct(data_ + used_, end());
    }
    used_ = new_used;
}

template <typename T, typename Alloc>
void Vec<T, Alloc>::push_back(const T& t) {
    if (used_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    std::construct_at(data_ + used_, t);
    ++used_;
}

template <typename T, typename Alloc>
template <typename... Args>
void Vec<T, Alloc>::emplace(const_iterator cit, Args&&... args) {
    auto it = begin() + (cit - cbegin());
    std::move_backward(it, data_ + used_, data_ + used_ + 1);
    std::construct_at(it, std::forward<Args>(args)...);
    ++used_;
}

void test_default_and_size_ctor() {
    Vec<int> v1;                 // default ctor
    assert(v1.size() == 10);     // you set default to size=10
    assert(v1.capacity() >= 10);

    Vec<std::string> v2(5);      // ctor with size
    assert(v2.size() == 5);
    assert(v2.capacity() >= 5);
}

void test_copy_ctor_and_assignment() {
    Vec<int> v1(3);
    v1.reserve(5);
    v1.resize(3);
    for (int i = 0; i < 3; ++i) {
        v1.begin()[i] = i + 1;
    }

    Vec<int> v2(v1); // copy ctor
    assert(v2.size() == 3);
    assert(v2.capacity() >= 3);
    for (int i = 0; i < 3; ++i) {
        assert(v2.begin()[i] == i + 1);
    }

    Vec<int> v3;
    v3 = v1; // copy assignment
    assert(v3.size() == 3);
    for (int i = 0; i < 3; ++i) {
        assert(v3.begin()[i] == i + 1);
    }
}

void test_move_ctor_and_assignment() {
    Vec<int> v1(3);
    for (int i = 0; i < 3; ++i) {
        v1.begin()[i] = i * 10;
    }
    size_t old_cap = v1.capacity();

    Vec<int> v2(std::move(v1)); // move ctor
    assert(v2.size() == 3);
    assert(v2.capacity() == old_cap);
    assert(v1.size() == 0); // moved-from should be empty

    Vec<int> v3;
    v3 = std::move(v2); // move assignment
    assert(v3.size() == 3);
    assert(v2.size() == 0);
}

void test_reserve_and_resize() {
    Vec<int> v(2);
    size_t old_cap = v.capacity();
    v.reserve(old_cap * 2);
    assert(v.capacity() >= old_cap * 2);

    v.resize(1); // shrink
    assert(v.size() == 1);
    v.resize(5); // grow
    assert(v.size() == 5);
}

void test_push_back_and_emplace() {
    Vec<std::string> v;
    v.resize(0);

    v.push_back("hello");
    v.push_back("world");
    assert(v.size() == 2);
    assert(v.begin()[0] == "hello");
    assert(v.begin()[1] == "world");

    v.emplace(v.begin() + 1, "middle");
    assert(v.size() == 3);
    assert(v.begin()[1] == "middle");
}

void test_swap() {
    Vec<int> a(2);
    a.begin()[0] = 1;
    a.begin()[1] = 2;

    Vec<int> b(3);
    b.begin()[0] = 10;
    b.begin()[1] = 20;
    b.begin()[2] = 30;

    a.swap(b);
    assert(a.size() == 3 && a.begin()[0] == 10);
    assert(b.size() == 2 && b.begin()[0] == 1);
}

int main() {
    test_default_and_size_ctor();
    test_copy_ctor_and_assignment();
    test_move_ctor_and_assignment();
    test_reserve_and_resize();
    test_push_back_and_emplace();
    test_swap();

    std::cout << "All Vec tests passed!\n";
    return 0;
}
