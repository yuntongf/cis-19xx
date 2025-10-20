#include <cassert>
#include <cstddef>
#include <iostream>
#include <list>
#include <optional>
#include <unordered_map>

template <typename Key, typename Value>
class LRU {
    using Iter = typename std::list<Key>::iterator;

   public:
    LRU(std::size_t capacity) : capacity_(capacity) {}

    std::optional<Value> tryGet(const Key& key) {
        auto iter = cache_.find(key);
        if (iter == cache_.end()) {
            return std::nullopt;
        }
        const auto [value, old_iter] = iter->second;

        // update recency
        l_.erase(old_iter);
        l_.push_back(key);
        auto new_iter = std::prev(l_.end());
        cache_[key].second = new_iter;

        return value;
    }

    std::optional<Value> put(const Key& key, const Value& value) {
        std::optional<Value> res = std::nullopt;
        auto iter = cache_.find(key);
        if (iter != cache_.end()) {
            auto& [old_value, old_iter] = iter->second;
            l_.erase(old_iter);
            cache_.erase(iter);
            res = old_value;
        } else if (full()) {
            // evict
            auto evict_key = l_.front();
            l_.pop_front();
            cache_.erase(evict_key);
        }
        l_.push_back(key);
        auto new_iter = std::prev(l_.end());
        cache_.insert({key, {value, new_iter}});
        return res;
    }

    template <typename... Args>
    void emplace(const Key& key, Args&&... args) {
        put(key, Value(std::forward<Args>(args)...));
    }

    [[nodiscard]] std::size_t size() const noexcept { return cache_.size(); }

    [[nodiscard]] bool full() const noexcept { return size() == capacity_; }

   private:
    std::size_t capacity_;
    std::list<Key> l_;
    std::unordered_map<Key, std::pair<Value, Iter>> cache_;
};

int main() {
    LRU<int, std::string> lru(3);

    // Insert some items
    lru.put(1, "one");
    lru.put(2, "two");
    lru.put(3, "three");

    assert(lru.size() == 3);
    assert(lru.tryGet(1).value() == "one");
    assert(lru.tryGet(2).value() == "two");

    lru.put(4, "four");
    assert(lru.size() == 3);

    assert(!lru.tryGet(3).has_value());

    assert(lru.tryGet(1).value() == "one");
    lru.put(5, "five");

    assert(!lru.tryGet(2).has_value());

    assert(lru.tryGet(1).value() == "one");
    assert(lru.tryGet(4).value() == "four");
    assert(lru.tryGet(5).value() == "five");

    lru.emplace(6, 6, 6);
    assert(lru.tryGet(6).has_value());

    std::cout << "All LRU cache tests passed!\n";
}
