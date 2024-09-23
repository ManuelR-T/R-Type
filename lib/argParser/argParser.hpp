/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** argParser
*/

#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

class ArgParser {
    public:
    enum class ArgType {
        BOOL,
        INT,
        UNSIGNED_INT,
        DOUBLE,
        STRING
    };

    struct Argument {
        std::string name;
        ArgType type;
        bool required;
        std::string description;
        std::optional<std::function<bool(const std::string &)>> validator;
        std::optional<std::string> value;
    };

    void addArgument(
        const std::string &name,
        ArgType type,
        bool required,
        const std::string &description,
        std::optional<std::function<bool(const std::string &)>> validator = std::nullopt
    );

    bool parse(int argc, char *argv[]);

    template <typename T>
    T getValue(const std::string &name) const;

    void printHelp() const;

    private:
    bool validateType(const std::string &value, ArgType type) const;
    std::string argTypeToString(ArgType type) const;

    std::unordered_map<std::string, Argument> arguments_;
};

template <typename T>
T ArgParser::getValue(const std::string &name) const
{
    auto it = arguments_.find(name);
    if (it != arguments_.end() && it->second.value) {
        T result;
        std::istringstream iss(*(it->second.value));
        iss >> result;
        if (iss.fail() || !iss.eof()) {
            throw std::runtime_error("Failed to convert argument value to the requested type for argument: " + name);
        }
        return result;
    }
    throw std::runtime_error("Argument is not defined or has no value: " + name);
}

template <>
inline bool ArgParser::getValue<bool>(const std::string &name) const
{
    auto it = arguments_.find(name);
    if (it != arguments_.end() && it->second.value) {
        std::string val = *(it->second.value);
        std::transform(val.begin(), val.end(), val.begin(), ::tolower);
        if (val == "true" || val == "1") {
            return true;
        } else if (val == "false" || val == "0") {
            return false;
        } else {
            throw std::runtime_error("Invalid boolean value for argument: " + name);
        }
    }
    throw std::runtime_error("Argument is not defined or has no value: " + name);
}
