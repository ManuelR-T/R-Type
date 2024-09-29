/*
** EPITECH PROJECT, 2024
** network
** File description:
** UDPClient
*/

#pragma once

#include "AsioClient.hpp"

#include <asio/ip/udp.hpp>
#include <thread>

using asio::ip::udp;

namespace ntw {

/**
 *  @class UDPClient
 *  @brief UDPClient is a class that connects to a udp server,
 *         and handle asynchronous operations (read, write) over it.
 */
class UDPClient : public AsioClient {
    public:
    /**
     * @brief Constructor takes the param host of the server and the param port
     *        where the udp socket will connect.
     * @param host Hostname of the server to connect.
     * @param port Port of the server the clients connect to.
     */
    UDPClient(const std::string &host, int port);

    /**
     * @brief Destructor of the UDPClient Object
     */
    ~UDPClient() override
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
     * @brief Send a data specified in param data of size param size
     *        to the endpoint variable member.
     * @param data data to send.
     * @param size Number of bytes of the data.
     */
    void send(const char *data, std::size_t size) override;

    private:
    /**
     * @brief Recursive loop of asynchronous operations (read, write),
     *        handling the read and write on udp server.
     */
    void _asioRun() override;

    /**
     * @brief Function that handle the message that was read in `buff_` variable member.
     * @param ec Asio error code to be decode.
     * @param bytes Number of bytes that were read.
     */
    void _handleRecv(asio::error_code ec, std::size_t bytes);

    udp::endpoint _endpoint;
    udp::socket _sock;
    std::array<char, BUFF_SIZE> _buff;
    std::thread _recvThread;
};
} // namespace ntw
