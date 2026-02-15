/**
 * @file option.hpp
 * @brief Defines CLI option representation and supported option types.
 *
 * This header introduces the Option structure used to describe
 * command-line options and the OptionType alias that represents
 * supported value types.
 */

#pragma once

#include <string>
#include <variant>

namespace Clixxi {

/**
 * @typedef OptionType
 * @brief Variant type representing supported option value types.
 *
 * Supported types:
 * - bool
 * - int
 * - float
 * - std::string
 *
 * This type can be used to store or represent parsed option values.
 */
using OptionType = std::variant<bool, int, float, std::string>;

/**
 * @struct Option
 * @brief Represents metadata for a command-line option.
 *
 * Stores the option name and its description.
 * The actual parsed value is handled by Context.
 */
struct Option {
    /**
     * @brief Constructs an option definition.
     * @param name Name of the option (without leading dashes).
     * @param desc Human-readable description of the option.
     */
    explicit Option(const std::string& name, const std::string& desc = "no desc")
        : option_name_(name), option_desc_(desc) {}
        
    std::string option_name_; /**< Option identifier. */
    std::string option_desc_; /**< Option description text. */
};

}  // namespace Clixxi