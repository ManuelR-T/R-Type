/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** tracked_exception
*/

#pragma once

#include <algorithm>
#include <exception>
#include <string>
#include <source_location>

namespace ecs {
class TrackedException : public std::exception {
    private:
    std::string _message;

    static std::string getFuncName(const std::string &func)
    {
        std::string funcName;

        for (int i = func.find_first_of('(') - 1; func[i] != ' ' && i >= 0; --i) {
            funcName.push_back(func[i]);
        }
        std::ranges::reverse(funcName.begin(), funcName.end());
        return funcName;
    }

    static std::string indentExcepetion(const std::string &str)
    {
        std::string result;

        for (char c : str) {
            result += c;
            if (c == '\n') {
                result += "│ ";
            }
        }
        return result;
    }

    [[nodiscard]]
    static std::string formatSrcLocation(const std::source_location &info)
    {
        return std::string("\n└ \033[1;95mexception throw from: \033[0;1m") + info.file_name() + ": \033[1m" +
            getFuncName(info.function_name()) + ": \033[1;96m" + std::to_string(info.line()) + "\033[0m";
    }

    public:
    explicit TrackedException(
        const std::string &message,
        std::source_location location = std::source_location::current()
    )
    {
        _message += indentExcepetion(message);
        _message += formatSrcLocation(location);
    }

    [[nodiscard]]
    const char *what() const noexcept override
    {
        return _message.c_str();
    }
};
} // namespace ecs
