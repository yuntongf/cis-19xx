#include "cache.hpp"

SlabCache::SlabCache(std::size_t block_size)
    : block_size_(block_size) {}

void* SlabCache::Allocate() {
    for (auto& s : slabs_) {
        if (!s->IsFull()) {
            return s->Allocate();
        }
    }
    slabs_.push_back(std::make_unique<Slab>(block_size_, 256));
    return slabs_.back()->Allocate();
}

bool SlabCache::Owns(void* ptr) const {
    for (auto& s : slabs_) {
        if (s->Owns(ptr)) {
            return true;
        }
    }
    return false;
}

void SlabCache::Deallocate(void* ptr) {
    for (auto& s : slabs_) {
        if (s->Owns(ptr)) {
            return s->Deallocate(ptr);
        }
    }
}
