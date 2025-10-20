cmake_minimum_required(VERSION 3.15)
project(ACappella LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -O2)
endif()

file(GLOB SRC_FILES
    ${CMAKE_SOURCE_DIR}/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(acappella ${SRC_FILES})

target_include_directories(acappella PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

find_package(SFML 2.5 COMPONENTS audio system REQUIRED)

target_link_libraries(acappella PRIVATE
    sfml-audio
    sfml-system
)