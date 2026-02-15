/**
 * @file exception.hpp
 * @brief Defines custom exception types used across the Clixxi framework.
 *
 * This header provides a hierarchy of runtime exceptions derived from
 * std::runtime_error. All exceptions are located in the Clixxi namespace
 * and are intended to describe CLI-related errors such as missing options,
 * invalid types, or unknown commands.
 */

#pragma once

#include <stdexcept>
#include <string>

namespace Clixxi {

/**
 * @struct Exception
 * @brief Base exception type for all Clixxi-related errors.
 *
 * Inherits from std::runtime_error and provides a unified
 * exception base for the framework.
 */
struct Exception : std::runtime_error {
    /**
     * @brief Constructs an exception with a message.
     * @param msg Error description.
     */
    explicit Exception(const std::string& msg) : std::runtime_error(msg) {}
    /**
     * @brief Constructs an exception with a C-string message.
     * @param msg Error description.
     */
    explicit Exception(const char* msg) : std::runtime_error(msg) {}
};

/**
 * @struct OptionNotFoundException
 * @brief Thrown when an option is requested but not registered.
 */
struct OptionNotFoundException : Exception {
    /**
     * @brief Constructs an exception for a missing option.
     * @param name Name of the option.
     */
    explicit OptionNotFoundException(const std::string& name) 
        : Exception("Option '" + name + "' not found") {}
};

/**
 * @struct MissingRequiredOptionException
 * @brief Thrown when a required option is not provided by the user.
 */
struct MissingRequiredOptionException : Exception {
    /**
     * @brief Constructs an exception for a missing required option.
     * @param name Name of the option.
     */
    explicit MissingRequiredOptionException(const std::string& name)
        : Exception("Missing required option '" + name + "'") {}
};

/**
 * @struct BadOptionTypeException
 * @brief Thrown when an option cannot be converted to the expected type.
 */
struct BadOptionTypeException : Exception {
    /**
     * @brief Constructs an exception for invalid option type conversion.
     * @param name Name of the option.
     * @param expected Expected type description.
     */
    explicit BadOptionTypeException(const std::string& name, const std::string& expected)
        : Exception("Option '" + name + "' cannot be converted to " + expected) {}
};

/**
 * @struct CommandNotFoundException
 * @brief Thrown when a command name is not registered in the application.
 */
struct CommandNotFoundException : Exception {
    /**
     * @brief Constructs an exception for an unknown command.
     * @param name Name of the command.
     */
    explicit CommandNotFoundException(const std::string& name) 
        : Exception("Command '" + name + "' not found") {}
};

/**
 * @struct CommandHasNotHandlerException
 * @brief Thrown when a command is executed without a registered handler.
 */
struct CommandHasNotHandlerException : Exception {
    /**
     * @brief Constructs an exception for a command without handler.
     * @param name Name of the command.
     */
    explicit CommandHasNotHandlerException(const std::string& name)
        : Exception("Command '" + name + "' has not handler") {}
};

}  // namespace Clixxi