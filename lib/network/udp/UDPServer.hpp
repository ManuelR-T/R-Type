/*
** EPITECH PROJECT, 2024
** network
** File description:
** UDPServer
*/

#pragma once

#include "../AsioServer.hpp"

#include <asio/error_code.hpp>
#include <asio/ip/address_v6.hpp>
#include <asio/ip/udp.hpp>
#include <cstring>
#include <map>

using asio::ip::udp;

namespace server {

    class UDPServer: public server::AsioServer {
    public:
        UDPServer(int port):
            endpoint_(udp::endpoint(udp::v4(), port)),
            sock_(io_, endpoint_)
        {
        }

        ~UDPServer() override = default;

        void run() override;

        void register_command(
            char const *name,
            std::function<void (char *, std::size_t)> func
        ) override;

        udp::socket &socket() { return sock_; };
        udp::endpoint &endpoint() { return endpoint_; };

        void handle_send(const std::vector<char> &vect);

    private:
        void asio_run() override;

        void handle_recv(asio::error_code ec, std::size_t bytes);

        udp::endpoint endpoint_;
        udp::socket sock_;
        char buff_[BUFF_SIZE];
        std::map<std::string, std::function<void (char *, std::size_t)>> handlers_;
    };
}
