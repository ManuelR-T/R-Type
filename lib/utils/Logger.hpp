/*
** EPITECH PROJECT, 2024
** my_cpplib
** File description:
** my_log
*/

#pragma once

#include <string>
#include <source_location>

namespace eng {
void log_info(const std::string &msg, std::source_location info = std::source_location::current());
void log_warning(const std::string &msg, std::source_location info = std::source_location::current());
void log_error(const std::string &msg, std::source_location info = std::source_location::current());
} // namespace eng
