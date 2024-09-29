/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** tracked_exception
*/

#pragma once

#include <exception>
#include <string>
#include <source_location>

namespace eng {

class TrackedException : public std::exception {
    private:
    std::string _message;

    std::string _getFuncName(const std::string &func);
    std::string _indentExcepetion(const std::string &str);
    std::string _formatSrcLocation(const std::source_location &info);

    public:
    explicit TrackedException(
        const std::string &message,
        std::source_location location = std::source_location::current()
    );

    [[nodiscard]] const char *what() const noexcept override;
};
} // namespace eng