#pragma once

#include <clixxi/context.hpp>
#include <functional>
#include <iostream>

namespace Clixxi {

class Command {
   public:
    Command(const std::string& name, const std::string& desc = "no desc") : name_(name), desc_(desc) {}
    Command& option(const std::string name, const std::string desc = "-") {
        options_.emplace(name, Option(name, desc));
        return *this;
    }
    Command& run(std::function<void(const Context&)> handler) {
        handler_ = std::move(handler);
        return *this;
    }
    void execute(const Context& context) const {
        if(context.has_option("help")) {
            std::cout << name_ << "help" << std::endl;
            return;
        }
        if(!handler_) {
            throw CommandHasNotHandlerException(name_);
        }
        handler_(context);
    }

   private:
    std::string name_;
    std::string desc_;
    std::function<void(const Context&)> handler_;
    std::map<std::string, Option> options_;
};
}  // namespace Clixxi