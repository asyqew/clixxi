/**
 * @file app.hpp
 * @brief Defines the main application entry point for the Clixxi CLI framework.
 *
 * The App class represents a CLI application. It manages:
 * - application metadata (name, description, version),
 * - command registration,
 * - argument parsing and command dispatching.
 */

#pragma once

#include <clixxi/command.hpp>
#include <iostream>

namespace Clixxi {

/**
 * @class App
 * @brief Represents a CLI application container.
 *
 * App is responsible for:
 * - registering commands,
 * - parsing raw CLI arguments,
 * - dispatching execution to the selected command.
 *
 * Example:
 * @code
 * Clixxi::App app("myapp", "Example CLI", "1.0");
 *
 * app.command("greet", "Print greeting")
 *     .run([](const Clixxi::Context& ctx) {
 *         std::cout << "Hello\n";
 *     });
 *
 * app.run(argc, argv);
 * @endcode
 */
class App {
   private:
    std::string name_;                        /**< Application name. */
    std::string desc_;                        /**< Application description. */
    std::string version_;                     /**< Application version. */
    std::map<std::string, Command> commands_; /**< Registered commands. */

    /**
     * @brief Extracts CLI arguments excluding executable name.
     *
     * @param argc Argument count.
     * @param argv Argument vector.
     * @return Vector of arguments starting from argv[1].
     */
    std::vector<std::string> parse_args(int argc, char* argv[]) {
        if (argc <= 1)
            return {};
        return std::vector<std::string>(argv + 1, argv + argc);
    }

   public:
    /**
     * @brief Constructs an application instance.
     *
     * @param name Application name.
     * @param desc Optional application description.
     * @param version Application version string.
     */
    App(const std::string& name, const std::string& desc = "", const std::string& version = "1.0")
        : name_(name), desc_(desc), version_(version) {}

    /**
     * @brief Registers or retrieves a command by name.
     *
     * If the command does not exist, it is created.
     * If it already exists, the existing instance is returned.
     *
     * @param name Command name.
     * @param desc Optional command description.
     * @return Reference to the command instance.
     */
    Command& command(const std::string& name, const std::string& desc = "") {
        auto [it, isInserted] = commands_.emplace(name, Command(name, desc));
        return it->second;
    }

    /**
     * @brief Runs the application.
     *
     * Parses CLI arguments and dispatches execution:
     * - "help" prints help placeholder
     * - "version" prints application version
     * - otherwise attempts to execute a registered command
     *
     * @param argc Argument count.
     * @param argv Argument vector.
     *
     * @throws `CommandNotFoundException` - If command is not registered.
     */
    void run(int argc, char* argv[]) {
        std::vector<std::string> args = parse_args(argc, argv);

        if (args.empty() || args.at(0) == "help") {
            std::cout << get_help();
            return;
        }

        if (args.at(0) == "version") {
            std::cout << name_ << " version " << version_;
            return;
        }

        auto it = commands_.find(args.at(0));

        if (it == commands_.end())
            throw CommandNotFoundException(args.at(0));

        it->second.execute(Context(std::vector<std::string>(args.begin() + 1, args.end())));
    }

    /**
     * @brief Returns application help information as a string.
     *
     * Constructs a detailed description of the application, including:
     * - name and brief description (if provided);
     * - usage template;
     * - list of available commands with their descriptions;
     * - hint on how to get help for a specific command;
     * - developer information.
     *
     * @return std::string A string containing formatted help information for the application.
     */
    std::string get_help() const {
        std::ostringstream oss;
        oss << name_;

        if (!desc_.empty())
            oss << " - " << desc_ << "\n";
        oss << "\n";

        oss << "Usage: " << name_ << " <COMMAND> [OPTIONS]\n\n";
        oss << "AVAILABLE COMMANDS:\n";

        for (const auto& [name, command] : commands_) {
            oss << "  " << std::left << std::setw(12) << name;
            if (!command.get_description().empty()) {
                oss << command.get_description();
            } else {
                oss << "no description";
            }
            oss << "\n";
        }

        oss << "\nSee '" << name_ << " <COMMAND> --help' to read about command.\n\n";
        oss << "This application created by Clixxi (https://github.com/asyqew/clixxi).\n";
        return oss.str();
    }
};

}  // namespace Clixxi