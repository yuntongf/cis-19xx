cmake_minimum_required(VERSION 3.15)
project(NewYearsHeap LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRC_FILES src/*.cpp)

add_executable(nyh
    main.cpp
    ${SRC_FILES}
)

target_include_directories(nyh PRIVATE include)

target_compile_options(nyh PRIVATE
    -Wall -Wextra -Wpedantic -O2
)

# Link SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(nyh PRIVATE sfml-graphics sfml-window sfml-system)
