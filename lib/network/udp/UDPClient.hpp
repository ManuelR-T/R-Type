/*
** EPITECH PROJECT, 2024
** network
** File description:
** UDPClient
*/

#pragma once

#include "AsioClient.hpp"
#include <asio/ip/udp.hpp>
#include <map>
#include <functional>
#include <iostream>
#include <thread>

using asio::ip::udp;

namespace client {
    class UDPClient: public AsioClient {
    public:
        UDPClient(const std::string &host, int port);

        ~UDPClient() override = default;

        void run() override;
        void send(const char *message, std::size_t size) override;

    private:
        void asio_run() override;
        void handle_recv(asio::error_code ec, std::size_t bytes);
        void handle_send(const std::vector<char> &vect);

        udp::endpoint endpoint_;
        udp::socket sock_;
        char buff_[BUFF_SIZE];
    };
}
