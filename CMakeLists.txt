# CMakeLists.txt — Build configuration for Clixxi (header-only C++ CLI framework)
# This file is used to compile examples and tests.
# The library itself is header-only, so no separate library target is built.

# Minimum required CMake version (modern C++ features support)
cmake_minimum_required(VERSION 3.14)

# Project name, version, and language
project(Clixxi
    VERSION 0.1.0
    DESCRIPTION "A header-only C++ framework for building CLI applications"
    HOMEPAGE_URL "https://github.com/SSSSOV/Clixxi"
    LANGUAGES CXX
)

# Set C++ standard: C++17 (widely supported and sufficient for most use cases)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Use only standard C++, not compiler extensions

# Enable warnings for better code quality (recommended for development)
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# --- Examples ---
# Build example applications to demonstrate how to use Clixxi

add_executable(example_hello examples/01_hello.cpp)
target_include_directories(example_hello PRIVATE include)
