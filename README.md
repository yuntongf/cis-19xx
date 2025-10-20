
# Rough draft of course website information

## Why should I take this class?
This class is designed for people who:
- have experience with assembly and C through a class like CIS 2400 or equivalent
- not afraid of getting into the weeds of details of a low-level system programming language
- looking to advance their system programming skills through learning a new language
- want to get practical experience with C++ through labs and projects
- welcome to interact with and learn from classmates during class, office hours, and in slack channel
- open to learn about the best C++ coding style and practices
- looking to use C++ in a practical setting such as internships and personal projects

This course provides a comprehensive introduction to C++, with some deep dive into certain core topics. By the end of the course, you will:
- learn about the basic, intermediate, and some advanced features of C++
- finish a series of lab exercises as in-class exercises or after-class assignments that are examples of C++ usage in the field of machine learning, gaming, trading, simulation, and audio processing.
- complete a technical interview style midterm
- complete a final project of your choosing with technical depth, which satisfy a set of functional and performance requirements achieved through test suite and performance benchmarking.
- work with other classmates on labs and projects
- know how to learn more about the language and where to look for new ideas and answer questions

## How to be successful in this class?
- I can't teach you everything about C++ so I want to teach you how to learn C++ (and hopefully make you love it:0). On my part, I'm going to try my best to
    - present topics in a investigative manner, and focus on the semantics rather than the syntax
    - put less information on the slides and make the lesson more interactive
    - walk you through the history and evolution of the language, its recent trends and future plans
    - show you good sources like the original working papers, good textbooks, conference talks, etc.
    - show you how to use good tools like compiler explorer to learn by toy examples
    - incorporate as much learning as possible into the labs. The labs will be relevant and designed with a specific application in mind.

- As a student, to take away as much as you can from the class. You should:
    - keep an open mind about programming languages, be curious about the peculiarities you see when learning C++
    - attend every lecture and interact with me during class. Even better - try to interact with your classmates during class and in slack (that's right we are using slack).
    - try to start early on the labs and go to OHs to ask questions
    - be curious about the language outside of the class. I understand you are all busy as hell but sometimes you will need to make sacrifices. The more you put in outside of the class the more you are going to learn - sometimes that's what takes you from a good programmer to a great one!
    - do not let go of any uncertainties, doubts or questions - they are the best source of learning. Try and see if you can answer them yourself first. If not, drop them in slack and go to OHs.

## Grading
### Pariticipation (10%)
- lecture attendence
- Q&A in Slack to make up for absences

### Labs (80%)
- These are meant to supplement lectures.
- They will be defined by a set of requirements (mostly functional but also some performance especially the later labs).
- I  will provide a style guide and you will be graded on style for the coding parts.
- The labs will be less and less guided as the semester progresses
- These are also a great source of inspiration for your projects!
- General plan (subject to change)
    - wordle game (variables and types, user defined types, member functions): a wordle clone with visual output!
    - tiny-tensor (template basics, template specialization, constexpr, compile vs run time): build a 2-2-1 neural net that can learn XOR problem from scratch
    - vector (templates, dynamic memory, object lifetime, STL, benchmarking): implement core of `std::vector`
    - new year's heap (dynamic memory, allocators, templates, STL containers): slab allocator that powers firework simulation
    - limit order book (class design, iterators, STL containers): implement an exchange order book and machine engine
    - a cappella! (multithreading, synchronization, OOP): audio processing engine that produces an A major chord with sound effects
    - One lab will be a technical interview during midterm time. This will serve as a checkpoint for both you and me to evaluate our progress on teaching/learning the class. I chose the in-person exam (or I'd like to call it technical interview) because it tests for real understanding and is more flexible. I will schedule this lab to be during midterm season - you don't have to spend as much time preparing for it if you pay attention during class, which this gives you more time to focus on other midterms.  
        - The reason why I didn't want to do a midterm project like the previous iteration of the classes is I want you to focus on learning in the first half. Plus you wouldn't have enough knowledge of C++ to finish a complete project that is relevant and meaningful. In my opinion, taking the time to ask questions, fiddle around and make mistakes in the lab exercises are more important than finishing a project in the first half of the course.

### Final Project (25%)
- technical depth
- some performance measurements
- some coding but does not have to be original
- solves a relavant and practical engineering problem
    - conference talks are a great source of inspiration
- presentation depends on how much time we have towards the end of the sem
    
## C++ Overview

### What's good about C++? (Need more content)
- A general-purpose language that is close to hardware and offers **zero-cost abstractions**
    - A programming language should serve to express a set of actions to be executed, and a set of concepts to be used when thinking about the problem (evolution p44). In other words, close to the metal (why it is based on C) while close to the problem (zero cost abstractions borrowed from Simula, Algol68)
- Performance and predictability
    - C++ gives you deterministic control over object lifetimes, memory layout, and resource management. Features like RAII, move semantics, and deterministic destructors make it ideal for systems that demand low latency and cannot afford garbage collection pauses.
    - Direct access to memory and system calls enables writing efficient concurrent and parallel code (thread pools, lock-free structures, custom allocators)
- Lots of freedom for the programmer - no spoonfeeding.
    - As safe and as efficient as you make it
- Interoperability and longevity.
    - C++ can interface with C, assembly, and other languages via FFI.
    - Decades of backward compatibility and a massive ecosystem make it suitable for extending legacy systems or integrating with hardware.

### Examples from usage in the industry (Need more content here)
- Embedded systems, Robotics (ROS)
- Server-side development, RPC framworks (gRPC, your CIS 5050)
- Gaming
- Video/audio processing
- Trading and financial infrastructure
- System programming (networking, drivers, operating systems)
---

### History
- Originated as “C with Classes”. Bjarne wanted something like C with data abstractions like classes in Simula.
- Bjarne talks about the standardization effort in his book The Design and Evolution of C++.
- **The Standard Committee** evolves the language:
  - C++98/03
  - C++11  
  - C++14 / C++17
  - C++20 / C++23
  - C++26

A **multi-paradigm federation of languages** (Need examples):
- **Subset of C** – close to metal, procedural
- **Object-oriented** – encapsulation, polymorphism, inheritance, lifetime control
- **Generic programming** – templates, metaprogramming
- **STL (containers + iterators + algorithms)** – functional-style abstractions

## Schedule (Dates are not accurate but are estimates)

| Week | Date | Topic | Lab start | Lab due |
|------|------|--------|-----------|-----------|
| 1 | Jan 16 | Course intro, types, user defined types | 
| 2 | Jan 23 | user defined types, simple inheritance, functions | Wordle | |
| 3 | Jan 30 | functions, template basics | tiny tensor | Wordle |
| 4 | Feb 6 | virtual functions and runtime polymorphism | | |
| 5 | Feb 13 | dynamic memory, smart pointers, exceptions | vector | tiny tensor |
| 6 | Feb 20 | Move semantics, perfect forwarding, The object lifetime | New Year's Heap | vector |
| 7 | Feb 27 | program lifetime, STL | Orderbook | |
| 8 | Mar 6 | STL design philosophy, tools and containers | | | 
| 9 | Mar 13 | Spring Break |  | New Year's Heap |
| 10 | Mar 20 | buffer time | final project | Orderbook |
| 11 | Mar 27 | advanced templates, currency & synchronization | a cappella | |
| 12 | Apr 3 | currency & synchronization, performance & safety features | | a cappella | 
| 13 | Apr 10 | testing & benchmarking, what you need to know about CPU & memory | | final project |
---

# List of topics to cover

## The fundamentals
### Variables and Types
- Primitive built-in types
- Variables
- Compound types: Pointers and references
- `const` qualifier
    - semantic meaning of const: read-only
    - influenced by operating systems protection mechanism

- User defined types: struct, class, union
    - could have been called `type` instead of `class`, but Bejarne took the term from Simula because he does not like inventing new terms
    - Data members
        - padding and memory alignment
    - Member functions
        - defined elsewhere so the linker technology works easier just like with C.
        - essetially passing an invisible `this` pointer to the function call (the object layout model p38 of evolution)
    - special member functions
        - constructor is created to allow the user to establish some invariant about the object at the beginning
        - copy constructors were created because it is inefficient to construct an object and then call assignment operator
    - public vs private access levels: protection against accident not fraud
    - explicit
    - const and reference overload of member functions
    - public and private inheritance
        - public inheritance inherents interface whereas private inherits implementation as implementation detail you want to hide
        - object layout of simple direct inheritance
- Auto type deduction
- Type cast: static_cast, const_cast, reinterpret_cast
- Value categories: Lvalues & rvalues
- Lvalue reference
- type alias
- namespaces

### Functions
- What is difference between an argument and a paramter?
- operator overload 
    - invented because default C way of assignment does not work for user defined classes like string or arrays
- overload resolution
- Lambdas and function objects: lambdas are syntax sugar for?
- inclining: started out as the programmer's decision but now largely taken over by compiler
- one definition rule
    - had roots in the choice of name equivalence over structural equivalence, with layout compatibility rules providing type casts supplied through structural equivalence

## Templates and metaprogramming
- why templates? try generic container classes without templates
- function templates
- class templates
- What is SFINAE?
- Template type deduction
- auto type deduction: how does it really work?
- auto return type, decltype

## Application of templates, types, user defined class: function objects
- function pointers
- function objects
- `std::function<>`
- closures and lambdas
- generic lambdas

## virtual functions, runtime polymorphism
- runtime polymorphism: Why does virtual exist? What does object memory layout and vtable look like for single, multiple and virtual inheritance?
- OOP Best practices: virtual destructures, containment vs inheritance, etc.
- As efficient as you make it
- RTTI

## Dynamic memory
- The OG `malloc()` and `free()`
- Operator new/delete
- Placement new/delete
- smart pointers initial intro
- Allocators

## Objects in C++ and the object lifetime
- objects are term invented by the standard to describe the semantics, the physical machine code does not know anything about the lifetime of an object and does not care
- what are objects in c++: why are references and fuctions not objects?
- what is lifetime?
    - allocated memory
    - initialized
    - used 
    - destroyed
    - deallocate memory
- storage duration
    - automatic
    - static
    - thread
- object lifetimes cases
    - variables
    - new / delete
    - temporary materialization conversion
    - temporary lifetime extension
    - placement new, construct_at, destroy_at
        - redefine a const object only creates UB if it has automatic, static, or thread duration; in other words, redefining a const object on the heap is fine.
        if you mess with an object that has automatic, static or thread storage duration, you need to put something in the object before its scope ends because the destructor will be called on it at the end of its scope
        - pointer and references to an objects can refer to the new object is old one is transparently replaced by the new object
        - if replacing a const heap object, need to use `std::launder` to 'refresh' the old pointer
    - implicit object creation
- constexpr and static

## The program lifetime
### build systems
- Make
- CMake
- Bazel

### Compile time
- Preprocessing: branching and text substitution
- Compiling
    - compiler basics: top-down and bottom-up compilers, recursive descent
    - lexical analysis
    - syntax analysis
    - semantic analysis
    - optimization
    - code generation
    - compiler warnings are conscious design decisions since C with classes rather than an after thought, do not ignore them!
- assembly
- linking

### Runtime
- program loading
    - allocate memory for sections
        - what goes into .text, .data, .rodata, .bss, stack and heap?
    - dynamic libraries
    - set up PCB
    - instruction ptr
- execution
    - start from _start
    - c++ run time initialization
    - run constructors on static data
    - handle exceptions
    - main(argc, argv)
    - call global destructors
    - clean up resources
    - return exit status

## The C++ standard library
- include the library headers when coding! So you know what comes from where:)

### std::chrono
- a good template class design for a discussion about standard library design philosophy 

### STL containers and algorithms
- Iterators
- ranges
- range based for loop
- Implementations of vector, list, deque, (multi)map/set, (multi) unordered_set/unordered_set
- What operations could potentially invalidate iterators and why do we care?
- What containers are cache friendly?
- some simple algorithms
- Implement an orderbook: map vs vector vs custom data structures? How to measure performance?

### utility library 
- `std::tuple` and `std::pair`

### vocabulary types
- `std::optional`
- `std::expected`
- `std::string_view`
- `std::span`

### I/O
- `std::in` and `std::out`
- filesystem

### Boost and abseil options
- show features that started in boost and made it to the standard eventually
- flat_map
- intrusive_ptr
- stable vector
- multi-index map

## Templates Redux
- Variadic templates, fold expressions
- Concepts
- What is reflection

## Exceptions & safety features
- Use const
- RAII, rule of five
- Exceptions family tree
- What are exception safety levels? What does it mean to be exception safe and exception neutral?
- What if I throw in a destructor? 
- Assertion
- make_unique vs unique_ptr (Effective Modern C++ Item 21)
- noexcept
- std::swap, std::exchange, copy-and-swap
- Think like a library author: how to write an exception safe and exception neutral vector class without throw?
- Scoped enums
- Attributes like [[nodiscard]]
- Std::optional, std::variant

## Concurrency
- Conditions variables
    - cv_.wait(lck, pred) is expanded by the compiler to while(!pred()) { cv.wait(); }
- Relationship between thread, async, packaged_task, future, promise
- Jthread, stop tokens
- How to implement a thread pool, strand executor, task scheduler
- atomics
    - now we can actually talk about how shared_ptrs are implemented
- Implement a semaphore using atomic
- Lock-free queue?
- What is async programming? How do coroutines map to state machines?
- Coroutines and range-based for loops

## Functional style programming and ranges
- How to transform code to functional style?
- Immutability and ranges
- What if I have exceptions?

## Performance features
real-time programming
latency vs throughput
- pass by copy optimization
- RVO, NRVO, prvalue vs xvalue, copy ellision in c++17
- avoid copies
- immutability: using span, const
- use move
- compile-time polymorphism instead of inheritance
- zero-cost abstraction
- use STL
- memory alignment
- min (alignof(T), std::max_align_t)
- `#pragma pack`
- row major vs col major
- cache awareness
- simd
- inline, noexcept
- hot path
- [[likely]] doesn't work as you think
- union
- bitfields
- memory pool

## Some resources
- books: 
    - C++ Primer
    - Effective C++
    - Effective Modern C++
    - Beginning c++23
    - The Design and Evolution of C++
    - C++ Memory Management
    - Eceptional C++
    - The C++ Standard Library
    - Learning the Boost C++ Library
    - Elements of Programming
- working papers from the standard committee
- cppreference.com
- conferences:
    - cppcon
    - c++ now
    - c++ on sea
    - Meeting C++
- podcasts
    - CppCast
    - cpp weekly
- C++ book reviews: https://ltcmelo.com/reviews.html


# Some questions I didn't have time to clarify

**What is your experience with C++?**

I gained most of my C++ experience through industry internships at SIG, Bloomberg, and TikTok. Working in these environments gave me the chance to write real-world C++ code and learn directly from experienced engineers who helped me appreciate the language. At Bloomberg, for example, I took lessons from Vittorio Romeo, a member of the C++ Standards Committee, who helped me understand the language at a much deeper level.

Outside of work, I’ve learned a lot from long-form content like textbooks, conference talks, and training sessions. For example, books like C++ Primer, Effective C++, the Design and Evolution of C++ have been a great source of learning for me. Earlier this year, I attended CppCon in Denver and took a training course with Fedor Pikus, whose talks I’ve followed for a while on YouTube. His insights into low-level systems programming and computer architecture really solidified my understanding of where C++ shines. I’m hoping to condense some of those lessons into a session I’ll teach at the end of the semester.

Over the summer, while preparing for recruiting, I also compiled a list of C++ interview questions - which I plan to use as a starting point for the in-person exam in the course.

**When do you graduate?**

I will graduate from my Master's in May 2026. I will start working full-time in New York after grad, so I would love to explore the possibility of commuting from New York and continuing teaching the course after graduation.

**What is your workload like for spring semester?**

I'm planning to take 4 cus, including MEAM 5200 Introduction to Robotics, ESE 5050 Feedback Control Design and Analysis, and two other electives (tentatively CIS 5450 Big Data Analytics and CIS 5190 Applied Machine Learning, which hopefully wouldn't be too much work since I've taken CIS 5200 before). I'm not involved in or planning to join any clubs or student groups.

I understand the concern for me not having enough time to devote to the class. For that, once the course is confirmed, I will work out a feasible timeline to implement the changes I'm proposing. I'm also open to having a co-instructor if you believe it is necessary.

**What do you want to do differently from previous iterations of the course?**

I will keep the format of the class for the most part. That is, the students will read some materials and learn about the topics ahead of the class, and I will present a lecture for 30min (mainly to clarify concepts and answer questions), and the rest of the time will be discussion, and in-class pair coding exercises. I think this is a good format because it allows for more active learning during class, and the students will have a chance to work on the labs during class time, which takes off some workload for them.

The previous iterations included quizzes that need to be completed by the beginning of the class. As a student I have personally always found these quizzes to be a bit annoying whenever I have them in a class, especially when they are completion based. I understand it is a way to encourage self-learning and to gauge weak points of the students before the class starts; but I also know a lot of stduents just use ChatGPT to answer them without paying any attention. Overtime they just become a formality for both the students and the instructor: the students do them to get the participation points and the instructors rarely check the results or cannot rely on the results of the quizzes due to the completion-based nature and the use of AI tools. I personally would do the quizzes in class after the 30min lecture and q&a time, and before the discussion and lab exercise time. This gives the students some time to learn or relearn the materials through the lecture and ask questions. In this way the results of the quizzes will shed more light onto which areas are weak, and the discussion and even lab portion of the class can be more directed toward these weak areas. In addition, I would make the quizzes point-based instead of completion based; but I would keep them as a very small portion of the participation score. In this way, the students will put in more effort before class and learn from the lectures more effectively, while not being stressed out about the stakes of the quiz scores in their final grade.

Non-exhaustive list of features I plan to cover that are not in the previous iterations:

- Object memory layout and vtable layout for inheritance with and without virtual functions, multiple inheritance, and maybe virtual inheritance: I think this is crucial for understanding virtual functions and polymorphism and their real cost
- Low level memory management like placement new, operator new overloading, custom allocators: this is where C++ shines; hopefully can help them appreciate low level programming in C++
- Object lifetime and program lifetime: help students have a better understanding of what an object is in C++ and how fine-grained control they can get out of it
- Some cool data structures like SPSC lock-free queues, conflation queues, thread pool, memory pool, implementation of `std::vector`, smart pointers, slab allocators: hese will be mostly integrated into labs. These low level data structures are highly relevant and knowing how they work and even implementing them can definitely lead to better grasp and appreciation of the language.
- more explanation on what could be done at compile time using constexpr
- C++ design philosophy. The Design and Evolution of C++ by Bjarne is a great source of inspiration. It outlined the history of C++ and detailed the principles it followed since its inception. The creator of C++ tried hard to keep away from parentalism and made no compromises when it comes to zero overhead of language features. The history of C++ is trully fascinating and knowing it really brings life to the language. Great class design in the STL such `std::chrono` are also a great source of learning. I hope to teach the students to appreciate good design when they see one.

Non-exhaustive list of features I'm thinking not to include:
- Intro to networking (Not the most relevant, also I think Travis might be covering this in his course)

**What is your plan to implement these changes?**
The additional work is mostly on:
- writing good reading material / slides for the new topics I'm planning to introduce
- reorganizing existing materials
- finish the labs
    - as you can see I already have some running project code that can produce some terminal/GUI/sound output, which is a good start
    - write test cases
    - more detailed READMEs including tips, hints, more detailed requirements, edges cases, extra credits, and submission

I'm planning to spend most of my time outside my internship this semester and winter break to work on these changes.

**What are the prerequisites for the class?**

CIS 2400. This is because I want them to have decent knowledge of assembly and C before learning a system programming language like C++. In addition, this allows us to move faster and get to the good parts faster than if I have to teach stduents about C first. I'm also not planning to review too much of C using class time.