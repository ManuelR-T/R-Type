/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** TCPClient
*/

#pragma once

#include "../AsioClient.hpp"

#include <asio.hpp>
#include <asio/ip/tcp.hpp>

using asio::ip::tcp;

namespace server {

class TCPClient : public client::AsioClient {
    public:
    /**
     * @brief Constructor takes the param host of the server and the param port
     *        where the udp socket will connect.
     * @param host Hostname of the server to connect.
     * @param port Port of the server the clients connect to.
     */
    TCPClient(const std::string &host, int port);

    /**
     * @brief Destructor of the TCPClient Object
     */
    ~TCPClient() override = default;

    /**
     * @brief Run the `io_context` variable member of this class and
     *        the `asio_run` function that is a recursive loop of asynchronous operations.
     */
    void run() override {};

    private:
    /**
     * @brief Recursive loop of asynchronous operations (read, write),
     *        handling the read and write on udp server.
     */
    void asio_run() override {};

    tcp::socket socket_;
    std::string host_;
    int port_;
};

} // namespace server
