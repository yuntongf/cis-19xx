#include <type_traits>

template <typename T>
constexpr auto move(T&& t) noexcept {
    return static_cast<std::remove_reference_t<T>&&>(t);
}

template <typename T>
auto forward(std::remove_reference_t<T>& t) {
    return static_cast<T&&>(t);
}

template <typename T>
auto forward(std::remove_reference_t<T>&& t) {
    return static_cast<T&&>(t);
}

template auto move(int&& t) noexcept;

int main() {
    return 0;
}