/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** TCPClient
*/

#pragma once

#include "AsioClient.hpp"

#include <asio.hpp>
#include <asio/ip/tcp.hpp>
#include <thread>

using asio::ip::tcp;

namespace client {

class TCPClient : public client::AsioClient {
    public:
    /**
     * @brief Constructor takes the param host of the server and the param port
     *        where the udp socket will connect.
     * @param host Hostname of the server to connect.
     * @param port Port of the server the clients connect to.
     */
    TCPClient(const std::string &host, int port, std::size_t size_data);

    /**
     * @brief Destructor of the TCPClient Object
     */
    ~TCPClient() override;

    /**
     * @brief Run the `io_context` variable member of this class and
     *        the `asio_run` function that is a recursive loop of asynchronous operations.
     */
    void run() override;

    void send(const char *data, std::size_t size) override;

    void register_handler(std::function<void(const char *, std::size_t)> handler);

    private:
    /**
     * @brief Recursive loop of asynchronous operations (read, write),
     *        handling the read and write on udp server.
     */
    void asio_run() override;

    std::thread thread_;
    tcp::socket socket_;
    std::string host_;
    int port_;
    std::size_t size_data_;
    std::array<char, BUFF_SIZE> buff_;
    std::function<void(const char *, std::size_t)> recv_handler_;
};

} // namespace client
