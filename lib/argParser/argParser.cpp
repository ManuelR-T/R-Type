/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** argParser
*/

#include "argParser.hpp"

void ArgParser::addArgument(
    const std::string &name,
    ArgType type,
    bool required,
    const std::string &description,
    std::optional<std::function<bool(const std::string &)>> validator
)
{
    arguments_[name] = {name, type, required, description, std::move(validator), std::nullopt};
}

bool ArgParser::parse(int argc, char *argv[])
{
    std::unordered_map<std::string, std::string> args;

    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arguments_.contains(arg)) {
            if (arguments_[arg].type == ArgType::BOOL) {
                args[arg] = "true";
            } else {
                if (i + 1 < argc) {
                    std::string value(argv[++i]);
                    args[arg] = value;
                } else {
                    std::cerr << "The argument " << arg << " expects a value." << std::endl;
                    return false;
                }
            }
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            return false;
        }
    }

    for (auto &[name, arg] : arguments_) {
        if (args.contains(name)) {
            std::string val = args[name];
            if (!validateType(val, arg.type)) {
                std::cerr << "Invalid type for argument " << name << ". Expected type: " << argTypeToString(arg.type)
                          << "." << std::endl;
                return false;
            }
            if (arg.validator && !(*arg.validator)(val)) {
                std::cerr << "Validation failed for argument " << name << "." << std::endl;
                return false;
            }
            arg.value = val;
        } else if (arg.required) {
            std::cerr << "Required argument " << name << " is missing." << std::endl;
            return false;
        } else if (arg.type == ArgType::BOOL) {
            arg.value = "false";
        }
    }

    return true;
}

void ArgParser::printHelp() const
{
    std::cout << "Available arguments:\n";
    for (const auto &[name, arg] : arguments_) {
        std::cout << "  " << name << " (" << argTypeToString(arg.type) << ") "
                  << (arg.required ? "[required]" : "[optional]") << " : " << arg.description << std::endl;
    }
}

bool ArgParser::validateType(const std::string &value, ArgType type) const
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

std::string ArgParser::argTypeToString(ArgType type) const
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
