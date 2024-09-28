/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** argParser
*/

#include "argParser.hpp"
#include <iostream>

void ArgParser::addArgument(
    const std::string &longName,
    std::optional<std::string> shortName,
    ArgType type,
    bool required,
    const std::string &description,
    std::optional<std::function<bool(const std::string &)>> validator
)
{
    _arguments.emplace_back(
        Argument{longName, shortName, type, required, description, std::move(validator), std::nullopt}
    );

    Argument *argPtr = &_arguments.back();

    _argMap[longName] = argPtr;

    if (shortName) {
        _argMap[*shortName] = argPtr;
    }
}

bool ArgParser::parse(int argc, const char **argv)
{
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);

        Argument *argument = nullptr;
        std::string argName;

        if (arg.rfind("--", 0) == 0) {
            argName = arg.substr(2);
            auto it = _argMap.find(argName);
            if (it != _argMap.end()) {
                argument = it->second;
            } else {
                std::cerr << "Unknown argument: --" << argName << std::endl;
                return false;
            }
        } else if (arg.rfind('-', 0) == 0) {
            argName = arg.substr(1);
            auto it = _argMap.find(argName);
            if (it != _argMap.end()) {
                argument = it->second;
            } else {
                std::cerr << "Unknown argument: -" << argName << std::endl;
                return false;
            }
        } else {
            std::cerr << "Invalid argument format: " << arg << std::endl;
            return false;
        }

        if (argument->type == ArgType::BOOL) {
            argument->value = "true";
        } else {
            if (i + 1 < argc) {
                std::string value(argv[++i]);
                argument->value = value;
            } else {
                std::cerr << "The argument " << arg << " expects a value." << std::endl;
                return false;
            }
        }
    }

    for (auto &arg : _arguments) {
        if (arg.value) {
            std::string val = *arg.value;
            if (!_validateType(val, arg.type)) {
                std::cerr << "Invalid type for argument " << arg.long_name
                          << ". Expected type: " << _argTypeToString(arg.type) << "." << std::endl;
                return false;
            }
            if (arg.validator && !(*arg.validator)(val)) {
                std::cerr << "Validation failed for argument " << arg.long_name << "." << std::endl;
                return false;
            }
        } else if (arg.required) {
            std::cerr << "Required argument --" << arg.long_name << " is missing." << std::endl;
            return false;
        } else if (arg.type == ArgType::BOOL && !arg.value.has_value()) {
            arg.value = "false";
        }
    }

    return true;
}

void ArgParser::printHelp() const
{
    std::cout << "Available arguments:\n";
    for (const auto &arg : _arguments) {
        std::cout << "  ";
        if (arg.short_name) {
            std::cout << "-" << *arg.short_name << ", ";
        }
        std::cout << "--" << arg.long_name << " (" << _argTypeToString(arg.type) << ") "
                  << (arg.required ? "[required]" : "[optional]") << " : " << arg.description << std::endl;
    }
}

bool ArgParser::_validateType(const std::string &value, ArgType type) const
{
    std::istringstream iss(value);
    if (type == ArgType::INT) {
        int x = 0;
        iss >> x;
        return !iss.fail() && iss.eof();
    } else if (type == ArgType::UNSIGNED_INT) {
        unsigned int x = 0;
        iss >> x;
        return !iss.fail() && iss.eof();
    } else if (type == ArgType::DOUBLE) {
        double x = 0.0;
        iss >> x;
        return !iss.fail() && iss.eof();
    } else if (type == ArgType::STRING) {
        return true;
    } else if (type == ArgType::BOOL) {
        std::string val = value;
        std::transform(val.begin(), val.end(), val.begin(), ::tolower);
        return val == "true" || val == "false" || val == "1" || val == "0";
    }
    return false;
}

std::string ArgParser::_argTypeToString(ArgType type) const
{
    switch (type) {
        case ArgType::INT:
            return "int";
        case ArgType::UNSIGNED_INT:
            return "unsigned int";
        case ArgType::DOUBLE:
            return "double";
        case ArgType::STRING:
            return "string";
        case ArgType::BOOL:
            return "bool";
        default:
            return "unknown";
    }
}
