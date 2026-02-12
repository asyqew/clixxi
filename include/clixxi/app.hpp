#pragma once

#include <clixxi/command.hpp>
#include <iostream>

namespace Clixxi {

class App {
   private:
    std::string name_;
    std::string desc_;
    std::string version_;
    std::map<std::string, Command> commands_;
    std::vector<std::string> parse_args(int argc, char* argv[]) {
        std::vector<std::string> args;
        for (int i = 1; i < argc; ++i) {
            args.push_back(argv[i]);
        }
        return args;
    }

   public:
    App(const std::string& name, const std::string& desc = "", const std::string& version = "1.0")
        : name_(name), desc_(desc), version_(version) {}

    Command& command(const std::string& name, const std::string& desc = "no desc") {
        auto [it, isInserted] = commands_.emplace(name, Command(name, desc));
        return it->second;
    }

    void run(int argc, char* argv[]) {
        std::vector<std::string> args = parse_args(argc, argv);

        if (args.empty() || args.at(0) == "help") {
            std::cout << "help";
            return;
        }

        if (args.at(0) == "version") {
            std::cout << version_;
            return;
        }

        auto it = commands_.find(args.at(0));

        if (it == commands_.end())
            throw CommandNotFoundException(args.at(0));

        it->second.execute(Context(std::vector<std::string>(args.begin() + 1, args.end())));
    }
};

}  // namespace Clixxi