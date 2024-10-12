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

namespace eng {

/**
 * @class ArgParser
 * @brief A command-line argument parser for handling and validating various types of arguments.
 *
 * The `ArgParser` class allows you to define expected command-line arguments, parse them from the input,
 * retrieve their values in the desired type, and generate help messages. It supports different argument types,
 * validation functions, and both long and short argument names.
 */
class ArgParser {
    public:
    /**
     * @enum ArgType
     * @brief Enumeration of supported argument types.
     */
    enum class ArgType {
        BOOL,         /**< Boolean values (`true`, `false`, `1`, `0`). */
        INT,          /**< Signed integer values. */
        UNSIGNED_INT, /**< Unsigned integer values. */
        DOUBLE,       /**< Floating-point numbers. */
        STRING        /**< String values. */
    };

    /**
     * @struct Argument
     * @brief Represents a single command-line argument.
     */
    struct Argument {
        std::string long_name;                 ///< The long name of the argument (e.g., "--help").
        std::optional<std::string> short_name; ///< The optional short name of the argument (e.g., "-h").
        ArgType type;                          ///< The type of the argument.
        bool required;                         ///< Indicates whether the argument is required.
        std::string description;               ///< A description of the argument for help messages.
        std::optional<std::function<bool(const std::string &)>> validator; ///< Optional custom validation function.
        std::optional<std::string> value; ///< The parsed value of the argument, if provided.
    };

    /**
     * @brief Adds a new argument to the parser.
     *
     * @param longName The long name of the argument (e.g., "help" for "--help").
     * @param shortName The optional short name of the argument (e.g., "h" for "-h"). Defaults to `std::nullopt`.
     * @param type The type of the argument. Defaults to `ArgType::STRING`.
     * @param required Whether the argument is required. Defaults to `true`.
     * @param description A description of the argument for help messages. Defaults to an empty string.
     * @param validator An optional custom validation function that takes the argument's string value and returns `true`
     * if valid. Defaults to `std::nullopt`.
     *
     * @throws std::invalid_argument If an argument with the same long or short name already exists.
     */
    void addArgument(
        const std::string &longName,
        std::optional<std::string> shortName = std::nullopt,
        ArgType type = ArgType::STRING,
        bool required = true,
        const std::string &description = "",
        std::optional<std::function<bool(const std::string &)>> validator = std::nullopt
    );

    /**
     * @brief Parses the command-line arguments.
     *
     * This function processes the input arguments, validates them, and stores their values.
     *
     * @param argc The number of command-line arguments.
     * @param argv The array of command-line argument strings.
     *
     * @return `true` if parsing is successful; `false` otherwise.
     *
     * @throws std::runtime_error If a required argument is missing or if validation fails.
     */
    bool parse(int argc, const char **argv);

    /**
     * @brief Retrieves the value of a parsed argument.
     *
     * This is a template function that converts the argument's string value to the desired type.
     * It throws an exception if the conversion fails or if the argument is not defined.
     *
     * @tparam T The type to convert the argument's value to.
     * @param longName The long name of the argument to retrieve.
     *
     * @return The value of the argument converted to type `T`.
     *
     * @throws std::runtime_error If the argument is not defined, has no value, or conversion fails.
     */
    template <typename T>
    T getValue(const std::string &longName) const
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

    /**
     * @brief Prints the help message listing all available arguments.
     *
     * This function outputs the descriptions of all defined arguments, including their
     * long and short names, whether they are required, and their descriptions.
     */
    void printHelp() const;

    private:
    /**
     * @brief Validates the type of an argument's value.
     *
     * Checks if the provided string value conforms to the expected `ArgType`.
     *
     * @param value The string value to validate.
     * @param type The expected type of the argument.
     *
     * @return `true` if the value matches the expected type; `false` otherwise.
     */
    bool _validateType(const std::string &value, ArgType type) const;

    /**
     * @brief Converts an `ArgType` to its string representation.
     *
     * @param type The argument type to convert.
     *
     * @return A string representing the argument type.
     */
    std::string _argTypeToString(ArgType type) const;

    std::list<Argument> _arguments;                      ///< A list of all defined arguments.
    std::unordered_map<std::string, Argument *> _argMap; ///< A map for quick lookup of arguments by name.
};

/**
 * @brief Specialized retrieval for boolean arguments.
 *
 * This specialization handles the conversion of string values to boolean.
 * It accepts "true", "false", "1", and "0" (case-insensitive).
 *
 * @param longName The long name of the boolean argument to retrieve.
 *
 * @return The boolean value of the argument.
 *
 * @throws std::runtime_error If the argument is not defined, has no value, or contains an invalid boolean string.
 */

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

} // namespace eng
