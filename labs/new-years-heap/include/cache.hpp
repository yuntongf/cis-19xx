#pragma once

#include "slab.hpp"

#include <memory>

class SlabCache {
public:
    explicit SlabCache(std::size_t block_size);

    void* Allocate();

    bool Owns(void* ptr) const;

    void Deallocate(void* ptr);
private:
    std::size_t block_size_;
    std::vector<std::unique_ptr<Slab>> slabs_;
};
