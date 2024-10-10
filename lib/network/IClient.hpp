/*
** EPITECH PROJECT, 2024
** network
** File description:
** IClient
*/

#pragma once

#include <cstddef>

namespace ntw {

/**
 * @class IClient
 * @brief An abstract interface for client implementations.
 *
 * The `IClient` class defines the essential functionalities that any client should implement
 * within the networking module. It serves as a contract ensuring that all derived client
 * classes provide implementations for running the client and sending data.
 */
class IClient {
    public:
    /**
     * @brief Constructs a new IClient object.
     *
     * Default constructor. Since `IClient` is an interface, this constructor is intended
     * to be called by derived classes.
     */
    IClient() = default;

    /**
     * @brief Destroys the IClient object.
     *
     * Virtual destructor to ensure proper cleanup of derived classes.
     */
    virtual ~IClient() = default;

    /**
     * @brief Starts the client's execution.
     *
     * This pure virtual function must be overridden by derived classes to define
     * the client's runtime behavior, such as initiating connections, handling events,
     * and managing the client's lifecycle.
     *
     * @throws std::runtime_error If the client fails to start correctly.
     */
    virtual void run() = 0;

    /**
     * @brief Sends data to the server.
     *
     * This pure virtual function must be overridden by derived classes to handle
     * the transmission of data from the client to the server. The function should ensure
     * that the data is correctly formatted and sent over the network.
     *
     * @param data Pointer to the data buffer containing the data to be sent.
     * @param size The size of the data buffer in bytes.
     *
     * @throws std::invalid_argument If the data pointer is null or the size is zero.
     * @throws std::runtime_error If the data fails to send successfully.
     */
    virtual void send(const char *data, std::size_t size) = 0;
};

} // namespace ntw
