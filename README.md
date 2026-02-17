# Clixxi

**Clixxi** — header-only C++17 framework for building command-line interface (CLI) applications.

The library provides a minimal and expressive API for:

* registering commands
* defining options
* parsing arguments
* accessing typed option values
* handling errors and warnings

Designed for lightweight CLI tools without external dependencies.


## Quick Start

### Minimal Example

```cpp
#include <clixxi/app.hpp>

int main(int argc, char* argv[]) {
    Clixxi::App app("demo", "Example CLI", "0.1.0");

    app.command("sum")
        .run([](const Clixxi::Context& ctx) {
            int a = ctx.get_option<int>("a");
            int b = ctx.get_option<int>("b");
            std::cout << "Result: " << a + b << std::endl;
        });

    app.run(argc, argv);
}
```

#### Run:

```bash
example_app.exe sum --a 1 --b 22
Result: 23
```

## Installation

### Option 1 — Add as subdirectory

```cmake
add_subdirectory(clixxi)
target_link_libraries(my_app PRIVATE clixxi)
```


### Option 2 — Install and use via `find_package`

#### Install

```bash
cmake -S . -B build
cmake --build build
cmake --install build
```

#### Use in another project

```cmake
find_package(clixxi REQUIRED)
target_link_libraries(my_app PRIVATE clixxi::clixxi)
```

## Requirements

* C++17 compatible compiler
* CMake ≥ 3.14

## Features

* Header-only (no linking required)
* Modern C++17 API
* Fluent command registration
* Typed option retrieval (`bool`, `int`, `float`, `std::string`)
* Automatic flag handling (`--flag`)
* Basic logging system (error / warning)
* Custom exception hierarchy
* CMake integration
* Zero third-party dependencies

## API Overview

### App

Main application container.

```cpp
Clixxi::App app("name", "description", "version");
app.run(argc, argv);
```

Responsibilities:

* register commands
* dispatch execution
* handle built-in `help` and `version`


### Command

Represents a CLI command.

```cpp
app.command("sum")
    .option("a")
    .option("b")
    .run([](const Clixxi::Context& ctx) {
        int a = ctx.get_option<int>("a");
        int b = ctx.get_option<int>("b");
        std::cout << a + b << std::endl;
    });
```


### Context

Provides typed access to parsed options.

Supported types:

* `bool`
* `int`
* `float`
* `std::string`

```cpp
int value = ctx.get_option<int>("number");
bool flag = ctx.get_option<bool>("verbose");
std::string text = ctx.get_option<std::string>("name", "default");
```


### Logger

Minimal logging utility.

```cpp
Clixxi::error << "Something failed" << std::endl;
Clixxi::warning << "Invalid value, using default" << std::endl;
```

Output is colorized (ANSI escape codes).


### Exceptions

All framework exceptions inherit from:

```
Clixxi::Exception
```

Main types:

* `CommandNotFoundException`
* `CommandHasNotHandlerException`
* `MissingRequiredOptionException`
* `BadOptionTypeException`


## CMake Configuration

Clixxi provides an `INTERFACE` target:

```cmake
add_library(clixxi INTERFACE)
```

Includes:

* C++17 requirement
* Include directories
* Install/export configuration


## Design Philosophy

Clixxi aims to be:

* Simple
* Lightweight
* Explicit
* Easy to integrate
* Free from external dependencies

It intentionally avoids complex metaprogramming and heavy abstractions.


## Versioning

Current version: **0.2.0**

Semantic Versioning is used:

```
MAJOR.MINOR.PATCH
```

* Breaking changes → MAJOR
* New features → MINOR
* Fixes → PATCH

## Roadmap

Planned improvements:

* `--key=value` syntax
* Short options (`-h`)
* Automatic help generation
* Required options
* Subcommands
* Extended type support
* Config file support
* Better validation system


## Status

Early development stage.

API may change before 1.0.0.
