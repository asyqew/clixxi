#pragma once

#include <string>
#include <variant>

namespace Clixxi {

using OptionType = std::variant<bool, int, float, std::string>;

struct Option {
    explicit Option(const std::string& name, const std::string& desc = "no desc") : option_name_(name), option_desc_(desc) {}
    std::string option_name_;
    std::string option_desc_;
};

}  // namespace Clixxi