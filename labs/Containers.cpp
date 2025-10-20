#include <deque>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <unordered_map>

template <typename T>
void print(const std::deque<T>& d) {
    std::copy(d.cbegin(), d.cend(), 
        std::ostream_iterator<T>(std::cout, ", "));
    std::cout << "\n";
}

int main() {
    std::deque<int> a;
    std::deque<int> b(2);
    std::deque<int> c(4, 9);
    std::deque<int> d{2};
    std::deque<int> e{4, 9};

    d.size();
    // d.capacity(); deque has no capacity
    d.empty();
    d.begin();
    d.end();
    d.cbegin();
    d.cend();
    d.rbegin();
    d.rend();
    d.crbegin();
    d.crend();

    d.push_back(4);
    d.push_front(5);
    d.emplace_back(5);
    d.emplace_front(4);
    
    auto it = d.insert(d.begin(), 3);
    it = d.emplace(d.cend(), 2);

    std::list<int> l1 = {2, 4, 6, 8};
    std::list<int> l2 = {5, 6, 9, 2};
    l1.sort();
    l2.sort();
    l1.merge({2, 5, 6, 2});

    std::list<int> l3 = {1, 2, 3, 4, 5};
    auto lit = std::next(l3.begin(), 2);
    std::list<int> l4 = {0, 0, 0};
    l3.splice(lit, l4, l4.begin(), l4.end());

    l4.remove(0);

    std::map<std::string, int> m = {
        {"a", 1},
        {"b", 2}
    };
    auto [m_it, inserted] = m.insert({"a", 2});
    m_it = m.insert(m.begin(), {"a", 2});

    // performance issue since we construct no matter what
    auto [m_it2, inserted2] = m.emplace("a", 2);

    struct Temp {
        int a = 0;
        int b = 0;

        Temp(int a, int b) {}

        bool operator==(const Temp& other) const {
            return a == other.a && b == other.b;
        }

        struct Hash {
            std::size_t operator()(const Temp& t) const noexcept {
                return std::hash<int>()(t.a) ^ (std::hash<int>()(t.b) << 1);
            }
        };
    };
    std::map<std::string, Temp> m2;
    // auto [it3, inserted3] = m2.emplace("a", 10, 33); doesn't work
    auto [it3, inserted3] = m2.emplace(
        std::piecewise_construct,
        std::forward_as_tuple("a"),
        std::forward_as_tuple(10, 44));
    
    auto [it4, inserted4] = m2.try_emplace("b", 2, 5);

    std::map<std::string, int> m3 = {
        {"a", 3},
        {"b", 5},
        {"b", 7},
        {"c", 9}
    };

    auto l = m3.lower_bound("b");
    auto r = m3.upper_bound("b");
    auto mit = m3.insert_or_assign("c", 10);
    std::cout << l->second << ", " << r->second;

    auto comp = [](const Temp& t) constexpr -> std::size_t {
        return std::hash<int>()(t.a) ^ std::hash<int>()(t.b);
    };

    // std::unordered_map<Temp, int, decltype(comp)> um(10, comp);
    std::unordered_map<Temp, int, Temp::Hash> um;
    um.insert({{2, 1}, 4});
}