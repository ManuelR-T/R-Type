/*
** EPITECH PROJECT, 2024
** network
** File description:
** UDPServer
*/

#pragma once

#include "AsioServer.hpp"

#include <array>
#include <asio/ip/udp.hpp>
#include <cstring>
#include <functional>

#include <thread>

#include <asio/error_code.hpp>
#include <asio/ip/address_v6.hpp>
#include <initializer_list>
#include <unordered_map>

using asio::ip::udp;

namespace ntw {

/**
 *  @class UDPServer
 *  @brief UDPServer is a server that handle udp network connections
 *         from remote clients.
 *
 *  The server handle commands that can be registered.
 */
class UDPServer : public ntw::AsioServer {
    /**
     * @brief Type that represent a client id useful to identify a specific client.
    */
    using client_id = size_t;

    public:
    /**
     * @brief Constructor takes the param port that the server listens,
     *        and construct the asynchronous udp server.
     * @param port Port the server listens.
     */
    UDPServer(int port) : _nbClients(0), _sock(_io, udp::endpoint(udp::v4(), port)) {}

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
     * @brief Return the udp endpoint of the last client that have connected.
     * @return Udp endpoint of type `udp::endpoint &`.
     */
    udp::endpoint &endpoint()
    {
        return _lastEndpoint;
    }

    /**
     * @brief Return all the clients endpoint that have connected to the udp server.
     * @return An unordered_map with the `client_id` and the client `endpoint` as value.
     */
    std::unordered_map<client_id, udp::endpoint> &endpoints()
    {
        return _cliEndpoints;
    }

    /**
     * @brief Send a message specified in @param data of size ( @param size ) to the @param endpoint parameter.
     * @param endpoint Remote endpoint where to send the data.
     * @param data Bytes to send.
     * @param size Number of bytes to send.
     */
    void send(udp::endpoint &endpoint, const char *data, std::size_t size);

    /**
     * @brief Send a message specified in @param data of size ( @param size ) to all the clients except to
     *        @param endpoint specified.
     * @param data Bytes to send.
     * @param size Number of bytes to send.
     * @param endpoint Endpoint that won't receive the data sent.
     */
    void sendAllExcept(const char *data, std::size_t size, udp::endpoint &endpoint);

    /**
     * @brief Send a message specified in @param data of size ( @param size ) to all the clients except to
     *        the @param endpoints specified.
     * @param data Bytes to send.
     * @param size Number of bytes to send.
     * @param endpoint Endpoint that won't receive the data sent.
     */
    void sendAllExcept(const char *data, std::size_t size, std::initializer_list<udp::endpoint> endpoints);

    private:
    /**
     * @brief Recursive loop of asynchronous operations (read, write),
     *        handling the read and write of udp clients.
     */
    void _asioRun() override;

    /**
     * @brief Function that handle the message that was read in `buff_` variable member,
     *        and interpret them for specific commands registered inside the server in `register_command` function.
     * @param endpoint Remote endpoint of the client that send the message.
     * @param ec Asio error code to be decode.
     * @param bytes Number of bytes that were read.
     */
    void _handleRecv(udp::endpoint &endpoint, asio::error_code ec, std::size_t bytes);

    /**
     * @brief Take an endpoint, stpre it if he is not in the list of current clients endpoints
     *
     * @param endpoint Endpoint to be stored in the list of clients endpoints.
     */
    void _handleEndpoint(udp::endpoint endpoint);

    udp::endpoint _lastEndpoint;
    std::unordered_map<client_id, udp::endpoint> _cliEndpoints;
    size_t _nbClients;
    udp::socket _sock;

    std::array<char, BUFF_SIZE> _buff;
    std::function<void(char *, std::size_t)> _handler;
    std::thread _recvThread;
};
} // namespace ntw
