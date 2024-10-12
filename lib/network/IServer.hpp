/*
** EPITECH PROJECT, 2024
** network
** File description:
** IServer
*/

#pragma once

namespace ntw {

/**
 * @class IServer
 * @brief An abstract interface for server implementations.
 *
 * The `IServer` class defines the essential functionalities that any server should implement
 * within the networking module. It serves as a contract ensuring that all derived server
 * classes provide implementations for running the server.
 */
class IServer {
    public:
    /**
     * @brief Constructs a new IServer object.
     *
     * Default constructor. Since `IServer` is an interface, this constructor is intended
     * to be called by derived classes.
     */
    IServer() = default;

    /**
     * @brief Destroys the IServer object.
     *
     * Virtual destructor to ensure proper cleanup of derived classes.
     */
    virtual ~IServer() = default;

    /**
     * @brief Starts the server's execution.
     *
     * This pure virtual function must be overridden by derived classes to define
     * the server's runtime behavior, such as initiating connections, handling client
     * requests, and managing the server's lifecycle.
     *
     * @throws std::runtime_error If the server fails to start correctly.
     */
    virtual void run() = 0;
};

} // namespace ntw
