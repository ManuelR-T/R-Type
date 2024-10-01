/*
** EPITECH PROJECT, 2024
** network
** File description:
** UDPServer
*/

#pragma once

#include "../AsioServer.hpp"

#include <array>
#include <asio/ip/udp.hpp>
#include <cstring>
#include <functional>

#include <thread>

#include <asio/error_code.hpp>
#include <asio/ip/address_v6.hpp>

using asio::ip::udp;

namespace server {

/**
 *  @class UDPServer
 *  @brief UDPServer is a server that handle udp network connections
 *         from remote clients.
 *
 *  The server handle commands that can be registered.
 */
class UDPServer : public server::AsioServer {
    public:
    /**
     * @brief Constructor takes the param port that the server listens,
     *        and construct the asynchronous udp server.
     * @param port Port the server listens.
     */
    UDPServer(int port) : _endpoint(udp::endpoint(udp::v4(), port)), _sock(_io, _endpoint) {}

    /**
     * @brief Destructor of the UDPServer Object
     */
    ~UDPServer() override
    {
        if (_recvThread.joinable()) {
            _io.stop();
            _recvThread.join();
        }
    }

    /**
     * @brief Run the `io_context` variable member of this class and
     *        the `asio_run` function that is a recursive loop of asynchronous operations.
     */
    void run() override;

    /**
     * @brief Register a command that is handled by the server.
     * @param func The function that handle the command that MUST have the following signature:
     *             `std::function<void (char *, std::size_t)>`
     */
    void registerCommand(std::function<void(char *, std::size_t)> func);

    /**
     * @brief Return the udp socket used for asynchronous operations.
     * @return Udp Socket of type `udp::socket &`.
     */
    udp::socket &socket()
    {
        return _sock;
    };

    /**
     * @brief Return the udp endpoint.
     * @return Udp endpoint of type `udp::endpoint &`.
     */
    udp::endpoint &endpoint()
    {
        return _endpoint;
    };

    /**
     * @brief Send a message specified in @param vect to the endpoint variable member.
     * @param vect Vector of string with the message to send.
     */
    void handleSend(const char *data, std::size_t size);

    private:
    /**
     * @brief Recursive loop of asynchronous operations (read, write),
     *        handling the read and write of udp clients.
     */
    void _asioRun() override;

    /**
     * @brief Function that handle the message that was read in `buff_` variable member,
     *        and interpret them for specific commands registered inside the server in `register_command` function.
     * @param ec Asio error code to be decode.
     * @param bytes Number of bytes that were read.
     */
    void _handleRecv(asio::error_code ec, std::size_t bytes);

    udp::endpoint _endpoint;
    udp::socket _sock;
    std::array<char, BUFF_SIZE> _buff;
    std::function<void(char *, std::size_t)> _handler;
    std::thread _recvThread;
};
} // namespace server
