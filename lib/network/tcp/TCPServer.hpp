/*
** EPITECH PROJECT, 2024
** network
** File description:
** TCPServer
*/

#pragma once

#include "../AsioServer.hpp"

#include <asio.hpp>
#include <asio/ip/tcp.hpp>
#include <list>

using asio::ip::tcp;

namespace server {

class Session : public std::enable_shared_from_this<Session> {
    public:
    Session(tcp::socket &&sock) : sock_(std::move(sock)) {}

    virtual ~Session() = default;

    void handle_client(std::function<void(tcp::socket &, char *, std::size_t)> &handler);

    tcp::socket &socket()
    {
        return sock_;
    }

    private:
    void handle_read(
        asio::error_code ec,
        std::size_t bytes,
        std::function<void(tcp::socket &, char *, std::size_t)> &handler
    );

    tcp::socket sock_;
    std::array<char, BUFF_SIZE> buff_;
};

class TCPServer : public server::AsioServer {
    public:
    TCPServer(int port);
    ~TCPServer() override;

    void run() override;

    void register_command(std::function<void(tcp::socket &, char *, std::size_t)> func);

    void sock_write(tcp::socket &sock, const char *data, std::size_t size);

    void remove_user(std::size_t id);

    void add_user(tcp::socket &sock, size_t id);

    void send_to_user(size_t id, const char *data, std::size_t size);

    void send_to_all_user(const char *data, std::size_t size);

    private:
    void asio_run();

    void handle_accept(asio::error_code ec, std::shared_ptr<Session> session);

    std::thread thread_;
    asio::io_context io_;
    tcp::acceptor acc_;
    std::function<void(tcp::socket &, char *, std::size_t)> handler_;
    std::unordered_map<size_t, std::shared_ptr<Session>> session_;
    std::list<std::shared_ptr<Session>> free_session_;
};
} // namespace server
