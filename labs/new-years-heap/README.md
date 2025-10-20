# New Year's Heap

In this lab we are going to create a multi-type generic slab allocator and use it to power a firework simulation.

Demo video: https://youtu.be/yhhbn1JJC6Q

general introduction of slab allocators: it's use in OS (linux), games, trading infra, high-performance computing, etc.

The structure of the slab allocator

The game frontend is already coded for you. Once you finish your slab allocator and pass all the test, you should be able to plug it into the GUI frontend and see some beautiful fireworks!

```
sudo apt install libsfml-dev
clang++ slab.cpp -o slab -lsfml-graphics -lsfml-window -lsfml-system
```

[TODO] think about how to help students debug this more effectively, maybe a heapmap for mem alloc?