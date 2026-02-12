#pragma once

#include <clixxi/exception.hpp>
#include <clixxi/option.hpp>
#include <clixxi/logger.hpp>
#include <map>
#include <vector>

namespace Clixxi {

class Context {
   private:
    std::map<std::string, std::string> options_;

   public:
    Context(const std::vector<std::string>& args) {
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

    template <typename T>
    T get_option(const std::string& name, const T& default_value) const {
        try{
            return get_option<T>(name);
        }
        catch (const MissingRequiredOptionException&) {
            return default_value;
        }
        catch (const BadOptionTypeException& e){
            Clixxi::warning << e.what() << std::endl;
            return default_value;
        }
    }

    bool has_option(const std::string& name) const { return options_.find(name) != options_.end(); }
};

}  // namespace Clixxi