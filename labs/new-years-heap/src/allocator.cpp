#include "allocator.hpp"

SlabCache& SlabAllocator::getCache(std::size_t sz) {
    if (!caches_.count(sz)) {
        caches_[sz] = std::make_unique<SlabCache>(sz);
    }
    return *caches_[sz];
}