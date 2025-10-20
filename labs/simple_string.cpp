#include <memory>
#include <cstddef>
#include <cstring>
#include <iostream>

// handle <=>

class SString {
public:
    SString(const char* str) {
        if (str == nullptr) {
            str = "";
        }
        size_ = std::strlen(str);
        data_ = std::make_unique<char[]>(size_ + 1);
        std::memcpy(data_.get(), str, size_ + 1);
    }

    SString(const SString& other) : size_(other.size_) {
        data_.reset(new char[other.size_ + 1]);
        std::memcpy(data_.get(), other.data_.get(), size_ + 1);
    }

    SString& operator=(const SString& other) {
        if (this != &other) {
            SString tmp(other);
            swap(tmp);
        }
        return *this;
    }
    SString(SString&&) noexcept = default;
    SString& operator=(SString&&) noexcept = default;
    ~SString() = default;

    [[nodiscard]] std::size_t size() const noexcept {
        return size_;
    }

    friend std::ostream& operator<<(std::ostream& os, SString& s) {
        os << s.data_.get();
        return os;
    }
private:
    void swap(SString& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }
private:
    std::unique_ptr<char[]> data_;
    std::size_t size_ = 0;
};

int main() {
    SString ss{""};
    
}