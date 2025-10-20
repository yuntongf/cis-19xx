# A Cappella!

In this final lab, you are going to implement a thread pool that will be used to parallelize audio processing. At the end of the lab you will be able to play a A major with volume and echo effect.

A thread pool is a concurrency design pattern that allows a fixed number of worker threads to execute multiple independent tasks without the overhead of constantly creating and destroying threads.

Instead of launching a new thread for every piece of work (which can be expensive in both time and memory), the thread pool maintains a set of worker threads that wait for tasks to be submitted to a shared queue. When a task arrives, one of the idle workers picks it up and executes it.

Once a worker finishes its task, it becomes available for the next one — enabling task parallelism and efficient utilization of system resources.

Your implementation will follow a standard producer-consumer pattern using synchronization primitives (`std::mutex`, `std::condition_variable`, and `std::queue`):

## Functional requirements

- Initialization
    - The constructor must create N worker threads, where N is the given capacity.
    - Each worker thread must continuously:
    - Wait for available tasks.
    - Execute them as they arrive.
    - Exit gracefully when the pool is stopped.

- Task Submission
    - The `submit()` method must:
    - Accept any callable (lambda, function, functor) and arbitrary arguments.
    - Package the callable inside a `std::packaged_task`, enabling result retrieval.
    - Return a `std::future<Result>` to allow asynchronous result access.
    - Enqueue the task in a thread-safe manner.
    - Notify exactly one worker thread waiting for tasks.

- Task Execution
    - Worker threads must correctly pop and execute tasks in FIFO order.
    - Tasks must execute concurrently across available threads.
    - Any exceptions thrown by a task must be propagated through the `std::future`.

- Synchronization & Thread Safety
    - All accesses to the task queue must be protected by a mutex.
    - The condition variable must efficiently block threads until:
    - A new task is available, or
    - The pool is being stopped.

- Graceful Shutdown
    - All worker threads must be notified and allowed to terminate naturally.
The destructor must join() all threads before exiting.
    - No dangling or unfinished threads should remain after destruction.

- Reusability
    - The pool should handle multiple sequential submissions even after previous tasks complete.
    - `submit()` should be callable from any thread (main or worker).