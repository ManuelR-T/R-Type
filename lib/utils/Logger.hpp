/*
** EPITECH PROJECT, 2024
** my_cpplib
** File description:
** my_log
*/

#pragma once

#include <string>

#if WIN32

namespace eng {
void logInfo(const std::string &msg);
void logWarning(const std::string &msg);
void logError(const std::string &msg);
} // namespace eng

#else

    #include <source_location>

namespace eng {
void logInfo(const std::string &msg, std::source_location info = std::source_location::current());
void logWarning(const std::string &msg, std::source_location info = std::source_location::current());
void logError(const std::string &msg, std::source_location info = std::source_location::current());
} // namespace eng

#endif
