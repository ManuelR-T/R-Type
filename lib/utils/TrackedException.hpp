#pragma once

#include <exception>
#include <string>

#ifdef _WIN32
// No source_location for Windows
#else
    #include <source_location>
#endif

namespace eng {

/**
 * @class TrackedException
 * @brief A custom exception class that tracks additional context information.
 *
 * The `TrackedException` class extends the standard `std::exception` to include
 * additional context such as the function name and source location where the exception
 * was thrown. This is particularly useful for debugging and logging purposes.
 */
class TrackedException : public std::exception {
    private:
    std::string _message; ///< The detailed message of the exception.

    /**
     * @brief Extracts the function name from a given string.
     *
     * This method processes the function signature to extract a readable function name.
     *
     * @param func The full function signature as a string.
     * @return A formatted function name.
     */
    std::string _getFuncName(const std::string &func);

    /**
     * @brief Formats the exception message with proper indentation.
     *
     * This method ensures that the exception message is neatly formatted for logging.
     *
     * @param str The original exception message.
     * @return An indented version of the exception message.
     */
    std::string _indentExcepetion(const std::string &str);

#ifdef _WIN32
    // If compiling on Windows, no source_location
#else
    /**
     * @brief Formats the source location information.
     *
     * This method creates a string representation of the source location where the exception was thrown.
     *
     * @param info The `std::source_location` information.
     * @return A formatted string containing the file name, line number, and function name.
     */
    std::string _formatSrcLocation(const std::source_location &info);
#endif

    public:
    /**
     * @brief Constructs a new `TrackedException` object.
     *
     * Initializes the exception with a message and, if available, the source location information.
     *
     * @param message The detailed message explaining the exception.
     #ifdef _WIN32
     *        Note: On Windows, source location information is not available.
     #else
     * @param location The source location where the exception was thrown. Defaults to the call site.
     #endif
     */
    explicit TrackedException(const std::string &message
#ifdef _WIN32
    // No source_location for Windows
#else
        , std::source_location location = std::source_location::current()
#endif
    );

    /**
     * @brief Retrieves the exception message.
     *
     * Overrides the `what()` method from `std::exception` to provide the custom message.
     *
     * @return A C-style character string containing the exception message.
     */
    [[nodiscard]] const char *what() const noexcept override;
};

} // namespace eng
