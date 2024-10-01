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

#if WIN32
    #include <mingw.thread.h>
#else
    #include <thread>
#endif

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
    TCPClient(const std::string &host, int port, std::size_t sizeData);

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

    void registerHandler(std::function<void(const char *, std::size_t)> handler);

    private:
    /**
     * @brief Recursive loop of asynchronous operations (read, write),
     *        handling the read and write on udp server.
     */
    void _asioRun() override;

    std::thread _thread;
    tcp::socket _socket;
    std::string _host;
    int _port;
    std::size_t _sizeData;
    std::array<char, BUFF_SIZE> _buff;
    std::function<void(const char *, std::size_t)> _recvHandler;
};

} // namespace client
