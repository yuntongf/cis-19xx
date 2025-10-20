#pragma once

#include "cache.hpp"

#include <unordered_map>

class SlabAllocator {
public:
    template <typename T, typename... Args>
    T* Allocate(Args&&... args) {
        auto& cache = getCache(sizeof(T));
        void* mem = cache.Allocate();
        return new (mem) T(std::forward<Args>(args)...);
    }

    template <typename T>
    void Deallocate(T* ptr) {
        ptr->~T();
        for (auto& [_, c] : caches_) {
            if (c->Owns(ptr)) {
                return c->Deallocate(ptr);
            }
        }
    }

private:
    SlabCache& getCache(std::size_t sz);
    std::unordered_map<std::size_t, std::unique_ptr<SlabCache>> caches_;
};