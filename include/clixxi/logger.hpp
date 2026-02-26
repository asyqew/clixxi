/**
 * @file logger.hpp
 * @brief Provides a simple logging interface for Clixxi.
 * The Logger class allows logging error and warning messages.
 * By default, it uses ConsoleLogger to print messages to stderr with color coding.
 * Users can set a custom logger by implementing the ILogger interface and calling Logger::set_logger().
 */

#pragma once
#include <memory>
#include <string>
#include <iostream>
#include <mutex>

namespace Clixxi {

/**
 * @class ILogger
 * @brief Interface for logging messages in Clixxi.
 * Defines methods for logging errors and warnings.
 * Users can implement this interface to create custom loggers.
 */
class ILogger {
   public:
    virtual ~ILogger() = default;
    virtual void error(const std::string& msg) = 0;
    virtual void warning(const std::string& msg) = 0;
};

/**
 * @class Logger
 * @brief Provides static access to the current logger instance.
 * By default, it uses ConsoleLogger which prints messages to stderr with color coding.
 */
class ConsoleLogger : public ILogger {
   public:
    /**
    * @brief Logs an error message to stderr with color coding.
    * The message is prefixed with "Clixxi error:" in red.
    * @param msg The error message to log.
    */
    void error(const std::string& msg) override { 
        std::cerr << "\033[1;31mClixxi error:\033[0m " << msg << std::endl; 
    }

    /**
     * @brief Logs a warning message to stderr with color coding.
     * The message is prefixed with "Clixxi warning:" in yellow.
     * @param msg The warning message to log.
     */
    void warning(const std::string& msg) override {
        std::cerr << "\033[1;33mClixxi warning:\033[0m " << msg << std::endl;
    }
};

/**
 * @class Logger
 * @brief Logger class that manages a global logger instance.
 */
class Logger {
   public:
    /**
    * @brief Sets the global logger instance.
    * Users can provide their own logger by implementing ILogger and calling this method.
    * @param logger A shared pointer to the custom logger instance.
    */
    static void set_logger(std::shared_ptr<ILogger> logger) { 
        instance() = std::move(logger); 
    }

    /**
     * @brief Retrieves the global logger instance.
     * If no logger has been set, it initializes and returns a default ConsoleLogger.
     * This method is thread-safe and ensures that only one instance of the logger is created.
     * @return Reference to the current ILogger instance.
     */
    static ILogger& get() {
        static std::mutex mtx;
        std::lock_guard<std::mutex> lock(mtx);
        if (!instance()) {
            instance() = create_default();
        }
        return *instance();
    }

   private:
    /**
     * @brief Internal storage for the global logger instance.
     * Uses a shared_ptr to allow for polymorphic loggers and easy replacement.
     * The instance is initialized lazily when get() is called for the first time.
     * @return Reference to the shared pointer holding the ILogger instance.
     */
    static std::shared_ptr<ILogger>& instance() {
        static std::shared_ptr<ILogger> logger;
        return logger;
    }

    /**
     * @brief Creates a default logger instance.
     * By default, this returns a ConsoleLogger that prints messages to stderr with color coding.
     * Users can override this method to provide a different default logger if desired.
     * @return A shared pointer to the default ILogger instance.
     */
    static std::shared_ptr<ILogger> create_default() { 
        return std::make_shared<ConsoleLogger>(); 
    }
};

}  // namespace Clixxi