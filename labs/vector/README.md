## Overview
In this lab, you will implement the core of the C++ Standard Library’s std::vector class — a dynamic array that automatically resizes as elements are added or removed.

A dynamic array starts with a fixed capacity, but when it becomes full and another element needs to be inserted, it allocates a new, larger block of memory, copies or moves the existing elements over, and then continues to grow from there.

In this lab, we’ll adopt a simple growth strategy: each time the array runs out of space, we double its capacity. Your CIS 1210 knowledge should tell you that this amortizes reallocation costs and provides efficient average-time insertions.

The name vector can be confusing because it also has a mathematical meaning. When the Standard Template Library (STL) was first introduced, the creators acknowledged that “vector” wasn’t the best choice for name, but the name stuck. Here, “vector” has no mathematical meaning - it simply means a contiguous, resizable sequence container.

## Functional requirements

You’ll implement the above functions to mimic the essential behavior of std::vector.

Here’s what each group of functions should do:

### Constructors and Destructor

**Constructor**: allocate space for a given initial size.

**Copy constructor**: perform deep copy — allocate new storage and copy-construct all elements.

**Move constructor**: “steal” ownership of the buffer from another vector.

**Destructor**: destroy all constructed elements and deallocate memory.

**Copy assignment**: copy contents from another vector; handle self-assignment safely.

**Move assignment**: transfer ownership of the internal buffer; release previous memory.

### Modifiers

**Reserve(new_cap)**: if new_cap > capacity_, allocate new memory, move (or copy) elements into it, free the old memory.

**Resize(new_size)**: change the number of elements — construct or destroy elements as needed.

**PushBack(const T&)**: insert a new element at the end; trigger reallocation if full.

**Emplace(it, Args&&...)**: construct an element in-place using placement new at a given iterator position.

**Clear()**: destroy all existing objects in the vector so it becomes empty.

## Hints and reminders
- Be very careful with object lifetimes. Construct objects with std::allocator_traits::construct, and destroy them with destroy.

- Don’t leak memory — every allocate should have a matching deallocate.

- Follow the Rule of Five — since the class manages a raw pointer.

- Always keep used_ <= capacity_.

- When you reallocate, prefer moving elements if possible (std::move_if_noexcept is your friend).

## Testing and Performance

Once your vector works correctly by passing the tests, you’ll measure the performance of its `PushBack()` function using two methods:

- raw time measurement using `std::chrono`
```
clang++ chrono_bench.cpp -o chrono_bench
```

- Google benchmark
```
sudo apt install libbenchmark-dev
clang++ google_bench.cpp -lbenchmark -lpthread -o google_bench
```

Now do some experiments using the benchmarks!