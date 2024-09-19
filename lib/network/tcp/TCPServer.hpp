/*
** EPITECH PROJECT, 2024
** network
** File description:
** TCPServer
*/

#pragma once

#include "../AsioServer.hpp"

#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>

using asio::ip::tcp;

namespace server {

    class Session : public std::enable_shared_from_this<Session> {
    public:
        Session(tcp::socket &sock): sock_(std::move(sock)) {}
        Session(asio::io_context &io): sock_(io) {}
        Session(tcp::socket &&sock): sock_(std::move(sock)) {}

        virtual ~Session() = default;

        void handle_client(std::function<void (char *, std::size_t)> &handler);
        tcp::socket &socket() { return sock_; }

    private:
        void handle_read(
            asio::error_code ec,
            std::size_t bytes,
            std::function<void (char *, std::size_t)> &handler
        );

        tcp::socket sock_;
        char buff_[BUFF_SIZE];
    };

    class TCPServer: public server::AsioServer {
    public:
        TCPServer(int port);
        ~TCPServer() override = default;

        void run() override;

        void register_command(
            char const *name,
            std::function<void (char *, std::size_t)> func
        );

        void sock_write(tcp::socket &sock_, std::string str);

    private:
        void asio_run() override;

        void handle_accept(asio::error_code ec, std::shared_ptr<Session> session);

        tcp::acceptor acc_;
        std::function<void (char *, std::size_t)> handlers_;
    };
}
