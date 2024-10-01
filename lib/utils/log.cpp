/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** log
*/

#include "log.hpp"
#include <iostream>


#if WIN32

void my::log::info(const std::string &msg)
{
    std::cerr << "\033[96minfo: \033[0m" << msg << std::endl;
}

void my::log::warning(const std::string &msg)
{
    std::cerr << "\033[95mwarning: \033[0m" << msg << std::endl;
}

void my::log::error(const std::string &msg)
{
    std::cerr << "\033[91merror: \033[0m" << msg << std::endl;
}


#else


#include <format>

void my::log::info(const std::string &msg, std::source_location info)
{
    std::cerr << std::format(
        "\033[1m{}:\033[96m{}\033[0;1m: "
        "\033[96minfo: \033[0m{}\n",
        info.file_name(),
        info.line(),
        msg
    );
}

void my::log::warning(const std::string &msg, std::source_location info)
{
    std::cerr << std::format(
        "\033[1m{}:\033[96m{}\033[0;1m: "
        "\033[95mwarning: \033[0m{}\n",
        info.file_name(),
        info.line(),
        msg
    );
}

void my::log::error(const std::string &msg, std::source_location info)
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
