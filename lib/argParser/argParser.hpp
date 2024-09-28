/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** argParser
*/

#pragma once

#include <algorithm>
#include <functional>
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
        const std::string &longName,
        std::optional<std::string> shortName = std::nullopt,
        ArgType type = ArgType::STRING,
        bool required = true,
        const std::string &description = "",
        std::optional<std::function<bool(const std::string &)>> validator = std::nullopt
    );

    bool parse(int argc, const char **argv);

    template <typename T>
    T getValue(const std::string &longName) const;

    void printHelp() const;

    private:
    bool _validateType(const std::string &value, ArgType type) const;
    std::string _argTypeToString(ArgType type) const;

    std::list<Argument> _arguments;
    std::unordered_map<std::string, Argument *> _argMap;
};

template <typename T>
T ArgParser::getValue(const std::string &longName) const
{
    auto it = std::find_if(_arguments.begin(), _arguments.end(), [&](const Argument &arg) {
        return arg.long_name == longName;
    });

    if (it != _arguments.end() && it->value) {
        T result;
        std::istringstream iss(*(it->value));
        iss >> result;
        if (iss.fail() || !iss.eof()) {
            throw std::runtime_error(
                "Failed to convert argument value to the requested type for argument: " + longName
            );
        }
        return result;
    }
    throw std::runtime_error("Argument is not defined or has no value: " + longName);
}

template <>
inline bool ArgParser::getValue<bool>(const std::string &longName) const
{
    auto it = std::find_if(_arguments.begin(), _arguments.end(), [&](const Argument &arg) {
        return arg.long_name == longName;
    });

    if (it != _arguments.end() && it->value) {
        std::string val = *(it->value);
        std::transform(val.begin(), val.end(), val.begin(), ::tolower);
        if (val == "true" || val == "1") {
            return true;
        } else if (val == "false" || val == "0") {
            return false;
        } else {
            throw std::runtime_error("Invalid boolean value for argument: " + longName);
        }
    }
    throw std::runtime_error("Argument is not defined or has no value: " + longName);
}
