#pragma once

#include <exception>
#include <string>

#ifdef _WIN32
// No source_location for Windows
#else
    #include <source_location>
#endif

namespace eng {

class TrackedException : public std::exception {
    private:
    std::string _message;

    std::string _getFuncName(const std::string &func);
    std::string _indentExcepetion(const std::string &str);

#ifdef _WIN32
    // If compiling on Windows, no source_location
#else
    std::string _formatSrcLocation(const std::source_location &info);
#endif

    public:
    explicit TrackedException(const std::string &message
#ifdef _WIN32
    // No source_location for Windows
#else
        , std::source_location location = std::source_location::current()
#endif
    );

    [[nodiscard]] const char *what() const noexcept override;
};
} // namespace eng
