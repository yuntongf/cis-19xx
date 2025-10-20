#include <cstddef>
#include <cstring>
#include <memory>

class String {
public:
    String(const char* str) : data_cap_{0} {
        std::size_t len = std::strlen(str);
        if (len <= SSO_CAP) {
            std::memcpy(sso_, str, len + 1);
        } else {
            data_ = static_cast<char*>(::operator new(len + 1));
            data_cap_ = len + 1;
            std::uninitialized_copy_n(str, len, data_);
        }
        size_ = len;
    }

    String(const String& other) : data_cap_{0} {
        if (size_ <= SSO_CAP) {
            std::uninitialized_copy_n(other.sso_, other.size_ + 1, sso_);
        } else {
            data_ = static_cast<char*>(::operator new(other.data_cap_));
            std::uninitialized_copy_n(other.data_, other.size_ + 1, data_);
            data_cap_ = other.data_cap_;
        }
        size_ = other.size_;
    }

    String& operator=(const String& other) {
        if (this != &other) {
            
        }
        return *this;
    }

    String(String&& other) noexcept;

    String& operator=(String&& other) noexcept;

    ~String() {
        if (data_cap_ > 0) {
            delete data_;
        }
    }

private:
    inline static constexpr std::size_t SSO_CAP = 15;
    std::size_t size_;
    std::size_t data_cap_;
    union {
        char* data_;
        char sso_[SSO_CAP + 1];
    };
};

int main() {

}