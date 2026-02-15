/**
 * @file context.hpp
 * @brief Provides argument parsing and typed option access for commands.
 *
 * The Context class represents parsed command-line arguments
 * passed to a specific command. It extracts options from raw
 * argument vectors and provides type-safe access to their values.
 */

#pragma once

#include <clixxi/exception.hpp>
#include <clixxi/logger.hpp>
#include <clixxi/option.hpp>
#include <map>
#include <vector>

namespace Clixxi {

/**
 * @class Context
 * @brief Represents parsed command execution context.
 *
 * Context is responsible for:
 * - parsing CLI arguments of the form `--key value`
 * - storing options internally as string key-value pairs
 * - providing typed access via `get_option<T>()`
 *
 * Supported value types:
 * - bool
 * - int
 * - float
 * - std::string
 */
class Context {
   private:
    /**
     * @brief Internal storage for parsed options.
     *
     * Keys are stored without leading dashes.
     * Values are stored as raw strings.
     */
    std::map<std::string, std::string> options_;

   public:
    /**
     * @brief Constructs a Context from raw command arguments.
     *
     * Expected option format:
     *   --key value
     *   --flag          (implicitly treated as true)
     *
     * Non-option arguments (not starting with "--") are ignored.
     *
     * @param args Vector of command arguments.
     */
    explicit Context(const std::vector<std::string>& args) {
        for (size_t i = 0; i < args.size(); ++i) {
            const std::string& arg = args.at(i);

            if (arg.substr(0, 2) != "--") {
                continue;
            }

            std::string key = arg.substr(2), value;

            if (i + 1 < args.size() && args.at(i + 1).substr(0, 2) != "--") {
                value = args.at(i + 1);
                ++i;
            } else {
                value = "true";
            }

            options_.emplace(key, value);
        }
    }

    /**
     * @brief Retrieves an option value converted to type T.
     *
     * @tparam T Desired return type.
     * @param name Option name.
     * @return Converted option value.
     *
     * @throws `MissingRequiredOptionException` - If option is not found and T is not bool.
     * @throws `BadOptionTypeException` - If conversion fails.
     */
    template <typename T>
    T get_option(const std::string& name) const {
        auto it = options_.find(name);

        if (it == options_.end()) {
            if constexpr (std::is_same_v<T, bool>) {
                return false;
            } else {
                throw MissingRequiredOptionException(name);
            }
        }

        const std::string& value = it->second;

        if constexpr (std::is_same_v<T, std::string>) {
            return value;
        } else if constexpr (std::is_same_v<T, bool>) {
            if (value == "true" || value == "1" || value == "on" || value == "yes") {
                return true;
            }
            if (value == "false" || value == "0" || value == "off" || value == "no") {
                return false;
            }
            throw BadOptionTypeException(name, "bool");
        } else if constexpr (std::is_same_v<T, int>) {
            try {
                size_t pos;
                int result = std::stoi(value, &pos);
                if (pos != value.size()) {
                    throw BadOptionTypeException(name, "int");
                }
                return result;
            } catch (...) {
                throw BadOptionTypeException(name, "int");
            }
        } else if constexpr (std::is_same_v<T, float>) {
            try {
                size_t pos;
                float result = std::stof(value, &pos);
                if (pos != value.size()) {
                    throw BadOptionTypeException(name, "float");
                }
                return result;
            } catch (...) {
                throw BadOptionTypeException(name, "float");
            }

        } else {
            throw BadOptionTypeException(name, "unknown type");
        }
    }

    /**
     * @brief Retrieves an option with a default fallback value.
     *
     * If the option is missing, returns default_value.
     * If type conversion fails, logs a warning and returns default_value.
     *
     * @tparam T Desired return type.
     * @param name Option name.
     * @param default_value Value to return if option is absent or invalid.
     * @return Converted option value or default.
     */
    template <typename T>
    T get_option(const std::string& name, const T& default_value) const {
        try {
            return get_option<T>(name);
        } catch (const MissingRequiredOptionException&) {
            return default_value;
        } catch (const BadOptionTypeException& e) {
            Clixxi::warning << e.what() << std::endl;
            return default_value;
        }
    }

    /**
     * @brief Checks whether an option exists.
     *
     * @param name Option name.
     * @return true if option was provided, false otherwise.
     */
    bool has_option(const std::string& name) const { return options_.find(name) != options_.end(); }
};

}  // namespace Clixxi