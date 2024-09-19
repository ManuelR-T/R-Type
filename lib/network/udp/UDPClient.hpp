/*
** EPITECH PROJECT, 2024
** network
** File description:
** UDPClient
*/

#pragma once

#include "AsioClient.hpp"

#include <asio/ip/udp.hpp>

using asio::ip::udp;

namespace client {

    /**
     *  @class UDPClient
     *  @brief UDPClient is a class that connects to a udp server,
     *         and handle asynchronous operations (read, write) over it.
    */
    class UDPClient: public AsioClient {
    public:
        /**
         * @brief Contructor takes the @param host of the server and the @param port
         *        where the udp socket will connect.
         * @param host Hostname of the server to connect.
         * @param port Port of the server the clients connect to.
        */
        UDPClient(const std::string &host, int port);

        /**
         * @brief Destructor of the UDPClient Object
        */
        ~UDPClient() override = default;

        /**
         * @brief Run the `io_context` variable member of this class and
         *        the `asio_run` function that is a recursive loop of asyncrounous operations.
        */
        void run() override;

        /**
         * @brief Send a message specified in @param message of size @param size
         *        to the endpoint variable member.
         * @param message Message to send.
         * @param size Number of bytes of the message.
        */
        void send(const char *message, std::size_t size) override;

    private:
        /**
         * @brief Recursive loop of asyncronous operations (read, write),
         *        handling the read and write on udp server.
        */
        void asio_run() override;

        /**
         * @brief Function that handle the message that was read in `buff_` variable member.
         * @param ec Asio error code to be decode.
         * @param bytes Number of bytes that were read.
        */
        void handle_recv(asio::error_code ec, std::size_t bytes);

        udp::endpoint endpoint_;
        udp::socket sock_;
        std::array<char, BUFF_SIZE> buff_;
    };
}
