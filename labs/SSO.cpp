#include <cstddef>
#include <cstring>
#include <utility>

class SSO {
public:
    SSO(const char* str) {
        InitStr(str);
    }

    SSO(const SSO& other) {
        CopyFrom(other);
    }

    SSO& operator=(const SSO& other) {
        if (this != &other) {
            CleanUp();
            CopyFrom(other);
        }
        return *this;
    }

    SSO(SSO&& other) {
        MoveFrom(std::move(other));
    }

    SSO& operator=(SSO&& other) {
        if (this != &other) {
            CleanUp();
            MoveFrom(std::move(other));
        }
        return *this;
    }

    ~SSO() {
        CleanUp();
    }

    const char* c_str() const {
        return is_sso_ ? content_ : str_;
    }
private:
    void InitStr(const char* str) {
        std::size_t length = strlen(str);
        if (length <= SSO_CAPACITY) {
            std::strcpy(content_, str);
            is_sso_ = true;
        } else {
            str_ = static_cast<char*>(::operator new(length + 1));
            std::strcpy(str_, str);
            is_sso_ = false;
        }
        length_ = length;
    }

    void MoveFrom(SSO&& other) {
        if (other.is_sso_) {
            std::strcpy(content_, other.content_);
            is_sso_ = true;
        } else {
            str_ = other.str_;
            is_sso_ = false;
        }
        length_ = other.length_;
    }

    void CopyFrom(const SSO& other) {
        if (other.is_sso_) {
            std::strcpy(content_, other.content_);
            is_sso_ = true;
        } else {
            str_ = static_cast<char*>(::operator new(other.length_ + 1));
            std::strcpy(str_, other.str_);
            is_sso_ = false;
        }
        length_ = other.length_;
    }

    void CleanUp() {
        if (!is_sso_) {
            ::operator delete(str_);
        }
    }
private:
    static constexpr std::size_t SSO_CAPACITY = sizeof(void*) - 1;
    std::size_t length_;
    union {
        char content_[SSO_CAPACITY + 1]; // +1 for null terminator
        char* str_;
    };
    bool is_sso_;
};

int main() {
    return 0;
}