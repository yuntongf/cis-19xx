#pragma once

#include <cstddef>
#include <vector>

class Slab {
public:
    Slab(std::size_t block_size, std::size_t block_count);

    ~Slab();

    void* Allocate();

    bool Owns(void* ptr) const;

    void Deallocate(void* ptr);

    [[nodiscard]] bool IsFull() const;

private:
    std::size_t block_size_;
    void* data_;
    std::vector<bool> used_;
};
