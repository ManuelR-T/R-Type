/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** log
*/

#include "Logger.hpp"
#include <iostream>

#if WIN32

void eng::info(const std::string &msg)
{
    std::cerr << "\033[96minfo: \033[0m" << msg << std::endl;
}

void eng::warning(const std::string &msg)
{
    std::cerr << "\033[95mwarning: \033[0m" << msg << std::endl;
}

void eng::error(const std::string &msg)
{
    std::cerr << "\033[91merror: \033[0m" << msg << std::endl;
}

#else

    #include <format>

void eng::info(const std::string &msg, std::source_location info)
{
    std::cerr << std::format(
        "\033[1m{}:\033[96m{}\033[0;1m: "
        "\033[96minfo: \033[0m{}\n",
        info.file_name(),
        info.line(),
        msg
    );
}

void eng::warning(const std::string &msg, std::source_location info)
{
    std::cerr << std::format(
        "\033[1m{}:\033[96m{}\033[0;1m: "
        "\033[95mwarning: \033[0m{}\n",
        info.file_name(),
        info.line(),
        msg
    );
}

void eng::error(const std::string &msg, std::source_location info)
{
    std::cerr << std::format(
        "\033[1m{}:\033[96m{}\033[0;1m: "
        "\033[91merror: \033[0m{}\n",
        info.file_name(),
        info.line(),
        msg
    );
}

#endif
