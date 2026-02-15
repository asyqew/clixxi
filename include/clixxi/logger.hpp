/**
 * @file logger.hpp
 * @brief Provides a minimal logging utility for error and warning output streams.
 *
 * This header defines the Clixxi::Logger class, which provides
 * lightweight logging facilities for CLI applications.
 *
 * Logging output is written to std::cerr and supports ANSI-colored
 * prefixes for different log levels.
 */

#pragma once
#include <iostream>
#include <string>

namespace Clixxi {

/**
 * @class Logger
 * @brief Static logging facility for error and warning messages.
 *
 * Logger provides two predefined log streams:
 * - `Clixxi::error`   — for error messages
 * - `Clixxi::warning` — for warning messages
 *
 * These streams behave similarly to std::ostream and support
 * chained operator<< calls.
 *
 * Example:
 * @code
 * Clixxi::error << "File not found" << std::endl;
 * Clixxi::warning << "Invalid argument, using default" << std::endl;
 * @endcode
 */
class Logger {
   public:
    /**
     * @class LogStream
     * @brief Internal stream wrapper used for formatted log output.
     *
     * LogStream acts as a proxy around `std::cerr`.
     * It ensures that a colored prefix is written once per message
     * before the actual log content.
     */
    class LogStream {
       public:
        /**
         * @enum Level
         * @brief Defines the severity level of the log message.
         */
        enum class Level {
            Error,  /**< Error level (red prefix). */
            Warning /**< Warning level (yellow prefix). */
        };

        /**
         * @brief Constructs a LogStream with a specified log level.
         * @param level Log severity level.
         */
        LogStream(Level level) : level(level), prefix_added(false) {}

        /**
         * @brief Stream insertion operator for generic types.
         *
         * Writes the value to std::cerr and ensures that the
         * corresponding prefix is added once per message.
         *
         * @tparam T Type of value to be streamed.
         * @param value Value to output.
         * @return Reference to the current LogStream instance.
         */
        template <typename T>
        LogStream& operator<<(const T& value) {
            ensurePrefix();
            std::cerr << value;
            return *this;
        }

        /**
         * @brief Stream insertion operator for stream manipulators.
         *
         * Supports usage of manipulators such as std::endl.
         *
         * @param manip Function pointer to stream manipulator.
         * @return Reference to the current LogStream instance.
         */
        LogStream& operator<<(std::ostream& (*manip)(std::ostream&)) {
            ensurePrefix();
            manip(std::cerr);
            return *this;
        }

       private:
        /**
         * @brief Writes the level-specific prefix if not already written.
         *
         * Ensures that the prefix appears only once per log message.
         */
        void ensurePrefix() {
            if (!prefix_added) {
                std::cerr << (level == Level::Error ? error_prefix : warning_prefix);
                prefix_added = true;
            }
        }

        Level level;       /**< Severity level of this stream. */
        bool prefix_added; /**< Tracks whether prefix has been printed. */

        /**
         * @brief ANSI-colored prefix for error messages.
         */
        static constexpr const char* error_prefix = "\033[1;31mClixxi error: \033[0m";
        /**
         * @brief ANSI-colored prefix for warning messages.
         */
        static constexpr const char* warning_prefix = "\033[1;33mClixxi warning: \033[0m";
    };

    /**
     * @brief Static error log stream instance.
     */
    inline static LogStream error{LogStream::Level::Error};
    /**
     * @brief Static warning log stream instance.
     */
    inline static LogStream warning{LogStream::Level::Warning};
};

/**
 * @brief Global alias for `Logger::error` stream.
 *
 * Allows usage:
 * @code
 * Clixxi::error << "Message";
 * @endcode
 */
inline Logger::LogStream& error = Logger::error;
/**
 * @brief Global alias for `Logger::warning` stream.
 *
 * Allows usage:
 * @code
 * Clixxi::warning << "Message";
 * @endcode
 */
inline Logger::LogStream& warning = Logger::warning;

}  // namespace Clixxi