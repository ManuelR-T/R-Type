/*
** EPITECH PROJECT, 2024
** my_cpplib
** File description:
** my_log
*/

#pragma once

#include <string>

#if WIN32

namespace my::log {
void info(const std::string &msg);
void warning(const std::string &msg);
void error(const std::string &msg);
} // namespace my::log

#else

#include <source_location>

namespace my::log {
void info(const std::string &msg, std::source_location info = std::source_location::current());
void warning(const std::string &msg, std::source_location info = std::source_location::current());
void error(const std::string &msg, std::source_location info = std::source_location::current());
} // namespace my::log

#endif