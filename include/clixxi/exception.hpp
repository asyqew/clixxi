#pragma once

#include <stdexcept>
#include <string>

namespace Clixxi {

struct Exception : std::runtime_error {
    explicit Exception(const std::string& msg) : std::runtime_error(msg) {}
    explicit Exception(const char* msg) : std::runtime_error(msg) {}
};

struct OptionNotFoundException : Exception {
    explicit OptionNotFoundException(const std::string& name) : Exception("Option '" + name + "' not found") {}
};

struct MissingRequiredOptionException: Exception {
    explicit MissingRequiredOptionException(const std::string& name) : Exception("Missing required option '" + name + "'") {}
};


struct BadOptionTypeException : Exception {
    explicit BadOptionTypeException(const std::string& name, const std::string& expected) : Exception("Option '" + name + "' cannot be converted to " + expected) {}
};

struct CommandNotFoundException : Exception {
    explicit CommandNotFoundException(const std::string& name) : Exception("Command '" + name + "' not found") {}
};

struct CommandHasNotHandlerException : Exception {
    explicit CommandHasNotHandlerException(const std::string& name) : Exception("Command '" + name + "' has not handler") {}
};

}  // namespace Clixxi