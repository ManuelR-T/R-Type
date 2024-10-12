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

/**
 * @brief Logs an informational message.
 *
 * This function logs a message with an "INFO" severity level.
 *
 * @param msg The message to be logged.
 */
void logInfo(const std::string &msg);

/**
 * @brief Logs a warning message.
 *
 * This function logs a message with a "WARNING" severity level.
 *
 * @param msg The message to be logged.
 */
void logWarning(const std::string &msg);

/**
 * @brief Logs an error message.
 *
 * This function logs a message with an "ERROR" severity level.
 *
 * @param msg The message to be logged.
 */
void logError(const std::string &msg);

} // namespace eng

#else

    #include <source_location>

namespace eng {

/**
 * @brief Logs an informational message with source location.
 *
 * This function logs a message with an "INFO" severity level and includes the source
 * location from where the log function was called.
 *
 * @param msg The message to be logged.
 * @param info The source location information. Defaults to the location where the function is called.
 */
void logInfo(const std::string &msg, std::source_location info = std::source_location::current());

/**
 * @brief Logs a warning message with source location.
 *
 * This function logs a message with a "WARNING" severity level and includes the source
 * location from where the log function was called.
 *
 * @param msg The message to be logged.
 * @param info The source location information. Defaults to the location where the function is called.
 */
void logWarning(const std::string &msg, std::source_location info = std::source_location::current());

/**
 * @brief Logs an error message with source location.
 *
 * This function logs a message with an "ERROR" severity level and includes the source
 * location from where the log function was called.
 *
 * @param msg The message to be logged.
 * @param info The source location information. Defaults to the location where the function is called.
 */
void logError(const std::string &msg, std::source_location info = std::source_location::current());

} // namespace eng

#endif
