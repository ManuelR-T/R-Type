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
class tracked_exception : public std::exception {
    private:
    std::string message_;

    static std::string get_func_name(const std::string &func)
    {
        std::string func_name;

        for (int i = func.find_first_of('(') - 1; func[i] != ' ' && i >= 0; --i) {
            func_name.push_back(func[i]);
        }
        std::ranges::reverse(func_name.begin(), func_name.end());
        return func_name;
    }

    static std::string indent_excepetion(const std::string &str)
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
    static std::string format_src_location(const std::source_location &info)
    {
        return std::string("\n└ \033[1;95mexception throw from: \033[0;1m") + info.file_name() + ": \033[1m" +
            get_func_name(info.function_name()) + ": \033[1;96m" + std::to_string(info.line()) + "\033[0m";
    }

    public:
    explicit tracked_exception(
        const std::string &message,
        std::source_location location = std::source_location::current()
    )
    {
        message_ += indent_excepetion(message);
        message_ += format_src_location(location);
    }

    [[nodiscard]]
    const char *what() const noexcept override
    {
        return message_.c_str();
    }
};
} // namespace ecs
