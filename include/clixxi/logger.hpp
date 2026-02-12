#pragma once
#include <iostream>
#include <string>

namespace Clixxi {

class Logger {
   public:
    class LogStream {
       public:
        enum class Level { Error, Warning };

        LogStream(Level level) : level(level), prefix_added(false) {}

        template <typename T>
        LogStream& operator<<(const T& value) {
            ensurePrefix();
            std::cerr << value;
            return *this;
        }

        LogStream& operator<<(std::ostream& (*manip)(std::ostream&)) {
            ensurePrefix();
            manip(std::cerr);
            return *this;
        }

       private:
        void ensurePrefix() {
            if (!prefix_added) {
                std::cerr << (level == Level::Error ? error_prefix : warning_prefix);
                prefix_added = true;
            }
        }

        Level level;
        bool prefix_added;

        static constexpr const char* error_prefix = "\033[1;31mClixxi error: \033[0m";
        static constexpr const char* warning_prefix = "\033[1;33mClixxi warning: \033[0m";
    };

    inline static LogStream error{LogStream::Level::Error};
    inline static LogStream warning{LogStream::Level::Warning};
};

inline Logger::LogStream& error = Logger::error;
inline Logger::LogStream& warning = Logger::warning;

}  // namespace Clixxi