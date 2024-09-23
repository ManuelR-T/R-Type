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
    UDPServer(int port) : endpoint_(udp::endpoint(udp::v4(), port)), sock_(io_, endpoint_) {}

    /**
     * @brief Destructor of the UDPServer Object
     */
    ~UDPServer() override = default;

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
    void register_command(std::function<void(char *, std::size_t)> func) override;

    /**
     * @brief Return the udp socket used for asynchronous operations.
     * @return Udp Socket of type `udp::socket &`.
     */
    udp::socket &socket()
    {
        return sock_;
    };

    /**
     * @brief Return the udp endpoint.
     * @return Udp endpoint of type `udp::endpoint &`.
     */
    udp::endpoint &endpoint()
    {
        return endpoint_;
    };

    /**
     * @brief Send a message specified in param vect to the endpoint variable member.
     * @param vect Vector of string with the message to send.
     */
    void handle_send(const std::vector<char> &vect);

    private:
    /**
     * @brief Recursive loop of asynchronous operations (read, write),
     *        handling the read and write of udp clients.
     */
    void asio_run() override;

    /**
     * @brief Function that handle the message that was read in `buff_` variable member,
     *        and interpret them for specific commands registered inside the server in `register_command` function.
     * @param ec Asio error code to be decode.
     * @param bytes Number of bytes that were read.
     */
    void handle_recv(asio::error_code ec, std::size_t bytes);

    udp::endpoint endpoint_;
    udp::socket sock_;
    std::array<char, BUFF_SIZE> buff_;
    std::function<void(char *, std::size_t)> handler_;
};
} // namespace server
