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
#include <list>
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
        std::string long_name;
        std::optional<std::string> short_name;
        ArgType type;
        bool required;
        std::string description;
        std::optional<std::function<bool(const std::string &)>> validator;
        std::optional<std::string> value;
    };

    void addArgument(
        const std::string &long_name,
        std::optional<std::string> short_name = std::nullopt,
        ArgType type = ArgType::STRING,
        bool required = true,
        const std::string &description = "",
        std::optional<std::function<bool(const std::string &)>> validator = std::nullopt
    );

    bool parse(int argc, char *argv[]);

    template <typename T>
    T getValue(const std::string &long_name) const;

    void printHelp() const;

    private:
    bool validateType(const std::string &value, ArgType type) const;
    std::string argTypeToString(ArgType type) const;

    std::list<Argument> arguments_;
    std::unordered_map<std::string, Argument *> arg_map_;
};

template <typename T>
T ArgParser::getValue(const std::string &long_name) const
{
    auto it = std::find_if(arguments_.begin(), arguments_.end(), [&](const Argument &arg) {
        return arg.long_name == long_name;
    });

    if (it != arguments_.end() && it->value) {
        T result;
        std::istringstream iss(*(it->value));
        iss >> result;
        if (iss.fail() || !iss.eof()) {
            throw std::runtime_error(
                "Failed to convert argument value to the requested type for argument: " + long_name
            );
        }
        return result;
    }
    throw std::runtime_error("Argument is not defined or has no value: " + long_name);
}

template <>
inline bool ArgParser::getValue<bool>(const std::string &long_name) const
{
    auto it = std::find_if(arguments_.begin(), arguments_.end(), [&](const Argument &arg) {
        return arg.long_name == long_name;
    });

    if (it != arguments_.end() && it->value) {
        std::string val = *(it->value);
        std::transform(val.begin(), val.end(), val.begin(), ::tolower);
        if (val == "true" || val == "1") {
            return true;
        } else if (val == "false" || val == "0") {
            return false;
        } else {
            throw std::runtime_error("Invalid boolean value for argument: " + long_name);
        }
    }
    throw std::runtime_error("Argument is not defined or has no value: " + long_name);
}
