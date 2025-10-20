#include "slab.hpp"


Slab::Slab(std::size_t block_size, std::size_t block_count)
        : block_size_(block_size),
          data_(::operator new(block_size * block_count)),
          used_(block_count, false) {}

Slab::~Slab() { ::operator delete(data_); }

void* Slab::Allocate() {
    for (std::size_t i = 0; i < used_.size(); ++i) {
        if (!used_[i]) { 
            used_[i] = true; 
            return static_cast<char*>(data_) + i * block_size_; 
        }
    }
    return nullptr;
}

bool Slab::Owns(void* ptr) const {
    auto start = static_cast<char*>(data_);
    auto end = start + block_size_ * used_.size();
    return ptr >= start && ptr < end;
}

void Slab::Deallocate(void* ptr) {
    auto idx = (static_cast<char*>(ptr) - static_cast<char*>(data_)) / block_size_;
    used_[idx] = false;
}

bool Slab::IsFull() const {
    for (bool u : used_) {
        if (!u) {
            return false;
        }
    }
    return true;
}
