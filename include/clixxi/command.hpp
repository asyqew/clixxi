/**
 * @file command.hpp
 * @brief Defines the Command class used to represent CLI commands.
 *
 * A Command encapsulates:
 * - command metadata (name, description),
 * - declared options,
 * - execution handler.
 */

#pragma once

#include <clixxi/context.hpp>
#include <functional>
#include <iomanip>
#include <iostream>

namespace Clixxi {

/**
 * @class Command
 * @brief Represents a single CLI command.
 *
 * A Command:
 * - may define options,
 * - must define a handler before execution,
 * - receives a Context object during execution.
 *
 * Example:
 * @code
 * app.command("sum")
 *     .option("a", "First number")
 *     .option("b", "Second number")
 *     .run([](const Clixxi::Context& ctx) {
 *         int a = ctx.get_option<int>("a");
 *         int b = ctx.get_option<int>("b");
 *         std::cout << a + b << std::endl;
 *     });
 * @endcode
 */
class Command {
   public:
    /**
     * @brief Constructs a command definition.
     *
     * @param name Command name.
     * @param desc Command description.
     */
    Command(const std::string& name, const std::string& desc = "") : name_(name), desc_(desc) {}

    /**
     * @brief Registers an option for the command.
     *
     * @param name Option name (without leading dashes).
     * @param desc Option description.
     * @return Reference to the current Command instance (fluent API).
     */
    Command& option(const std::string name, const std::string desc = "") {
        options_.emplace(name, Option(name, desc));
        return *this;
    }

    /**
     * @brief Assigns an execution handler to the command.
     *
     * The handler receives a parsed Context object.
     *
     * @param handler Function to execute when command is invoked.
     * @return Reference to the current Command instance (fluent API).
     */
    Command& run(std::function<void(const Context&)> handler) {
        handler_ = std::move(handler);
        return *this;
    }

    /**
     * @brief Executes the command.
     *
     * If the "help" option is present, prints a minimal help message.
     * Otherwise invokes the registered handler.
     *
     * @param context Parsed execution context.
     *
     * @throws `CommandHasNotHandlerException` - If no handler was assigned.
     */
    void execute(const Context& context) const {
        if (context.has_option("help")) {
            std::cout << get_help() << std::endl;
            return;
        }
        if (!handler_) {
            throw CommandHasNotHandlerException(name_);
        }
        handler_(context);
    }

    /**
     * @brief Returns the command description.
     *
     * @return std::string A string with the command description, or an empty string if no description was set.
     */
    std::string get_description() const { return desc_; }

    /**
     * @brief Returns the command description.
     *
     * @return std::string A string with the command description, or an empty string if no description was set.
     */
    std::string get_help() const {
        std::ostringstream oss;

        oss << "Command: " << name_ << "\n";
        if (!desc_.empty()) {
            oss << "Description: " << desc_ << "\n\n";
        }

        oss << "Usage: <PROGRAM> " << name_;
        if (!options_.empty()) {
            oss << " [OPTIONS]\n\n";
            oss << "OPTIONS:\n";
        }
        for (const auto& [name, option] : options_) {
            oss << "  --" << std::left << std::setw(10) << name;
            if (!option.option_desc_.empty())
                oss << "" << option.option_desc_;
            else
                oss << "No description.";
            oss << "\n";
        }
        return oss.str();
    }

   private:
    std::string name_;                            /**< Command name. */
    std::string desc_;                            /**< Command description. */
    std::function<void(const Context&)> handler_; /**< Execution handler. */
    std::map<std::string, Option> options_;       /**< Registered options. */
};
}  // namespace Clixxi