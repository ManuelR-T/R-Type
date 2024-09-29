/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** TrackedException
*/

#include "TrackedException.hpp"
#include <algorithm>

std::string eng::TrackedException::_getFuncName(const std::string &func)
{
    std::string funcName;

    for (int i = func.find_first_of('(') - 1; func[i] != ' ' && i >= 0; --i) {
        funcName.push_back(func[i]);
    }
    std::ranges::reverse(funcName.begin(), funcName.end());
    return funcName;
}

std::string eng::TrackedException::_indentExcepetion(const std::string &str)
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

std::string eng::TrackedException::_formatSrcLocation(const std::source_location &info)
{
    return std::string("\n└ \033[1;95mexception throw from: \033[0;1m") + info.file_name() + ": \033[1m" +
        _getFuncName(info.function_name()) + ": \033[1;96m" + std::to_string(info.line()) + "\033[0m";
}

eng::TrackedException::TrackedException(const std::string &message, std::source_location location)
{
    _message += _indentExcepetion(message);
    _message += _formatSrcLocation(location);
}

const char *eng::TrackedException::what() const noexcept
{
    return _message.c_str();
}
