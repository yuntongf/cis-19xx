#include <vector>
#include <iostream>
#include <iterator>

int main() {
    // std::vector<int> a;                      // default constructor
    // std::vector<int> b(5);                   // 5 default-initialized ints (0)
    // std::vector<int> c(5, 42);               // 5 elements, all 42
    // std::vector<int> d{1, 2, 3, 4};          // initializer list
    // std::vector<int> e(d);                   // copy constructor
    // std::vector<int> f(std::move(d));        // move constructor

    std::vector<int> v = {1, 2, 3};
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {4, 5, 6};

    // Check property
    std::cout << v.size();
    std::cout << v.capacity();
    std::cout << v.empty();

    // Storage
    v.reserve(100);
    v.resize(10);
    v.shrink_to_fit();

    // point
    std::cout << v.front() << "\n";
    std::cout << v.back() << "\n";
    std::cout << v[2] << "\n";
    std::cout << v.at(2) << "\n";

    // point
    v.push_back(4);
    v.emplace_back(10);
    auto res = v.insert(v.begin(), 0);
    res = v.emplace(v.begin(), 20);
    v[1] = 100;
    v.at(2) = 200;
    v.pop_back();
    res = v.erase(v.begin());

    // range
    res = v.insert(v.end(), 2, 9);
    res = v.insert(v.end(), {5, 10});
    res = v.insert(v.end(), a.begin(), a.end());
    res = v.erase(v.begin(), v.begin() + 2);

    // full range
    v.assign(5, 7);
    v.assign({1, 2, 3});
    v.assign(a.begin(), a.end());
    v.clear();
    a.swap(b);
    std::swap(a, b); 

    // Comparison
    if (a == b) std::cout << "equal\n";
    if (a < b) std::cout << "lexicographically less\n";

    // Replace [a.begin()+2, a.begin()+5) with b
    auto first = a.begin() + 2;  // points to 3
    auto last  = a.begin() + 5;  // points to 6

    auto pos = a.erase(first, last);  // removes 3, 4, 5 — returns iterator to 6
    a.insert(pos, b.begin(), b.end()); // insert b at that position

    std::copy(a.cbegin(), a.cend(), std::ostream_iterator<int>(std::cout, " "));
}