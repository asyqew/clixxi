# CMakeLists.txt — Build configuration for Clixxi (header-only C++ CLI framework)
# This file is used to compile examples and tests.
# The library itself is header-only, so no separate library target is built.

# Minimum required CMake version (modern C++ features support)
cmake_minimum_required(VERSION 3.14)

# Project name, version, and language
project(Clixxi
    VERSION 0.1.0
    DESCRIPTION "A modern, header-only C++ framework for building CLI applications"
    HOMEPAGE_URL "https://github.com/yourname/Clixxi"
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

add_executable(example_args examples/02_args.cpp)
target_include_directories(example_args PRIVATE include)

add_executable(example_subcommands examples/03_subcommands.cpp)
target_include_directories(example_subcommands PRIVATE include)

add_executable(example_options examples/04_options.cpp)
target_include_directories(example_options PRIVATE include)

# --- Tests ---
# Optional: add unit tests (you can use Catch2, doctest, etc.)
# Here's an example using built-in CTest

enable_testing()

add_executable(test_parser tests/test_parser.cpp)
target_include_directories(test_parser PRIVATE include)
add_test(NAME test_parser_run COMMAND test_parser)

add_executable(test_app tests/test_app.cpp)
target_include_directories(test_app PRIVATE include)
add_test(NAME test_app_run COMMAND test_app)

# --- Installation (optional) ---
# Install headers so users can do: find_package(Clixxi)
install(DIRECTORY include/clixxi DESTINATION include)
install(FILES single_include/clixxi.hpp DESTINATION include)

# Export configuration for find_package(Clixxi)
install(EXPORT ClixxiTargets
    DESTINATION lib/cmake/Clixxi
)

# --- Developer-friendly tip ---
# Users can include Clixxi in their projects via:
#   add_subdirectory(vendor/Clixxi)
#   target_link_libraries(myapp PRIVATE Clixxi::Clixxi)  # if you create an interface library
