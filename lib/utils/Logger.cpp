/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** log
*/

#include "Logger.hpp"
#include <format>
#include <iostream>

void eng::logInfo(const std::string &msg, std::source_location info)
{
    std::cerr << std::format(
        "\033[1m{}:\033[96m{}\033[0;1m: "
        "\033[96minfo: \033[0m{}\n",
        info.file_name(),
        info.line(),
        msg
    );
}

void eng::logWarning(const std::string &msg, std::source_location info)
{
    std::cerr << std::format(
        "\033[1m{}:\033[96m{}\033[0;1m: "
        "\033[95mwarning: \033[0m{}\n",
        info.file_name(),
        info.line(),
        msg
    );
}

void eng::logError(const std::string &msg, std::source_location info)
{
    std::cerr << std::format(
        "\033[1m{}:\033[96m{}\033[0;1m: "
        "\033[91merror: \033[0m{}\n",
        info.file_name(),
        info.line(),
        msg
    );
}
