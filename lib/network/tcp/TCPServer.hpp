/*
** EPITECH PROJECT, 2024
** network
** File description:
** TCPServer
*/

#pragma once

#include <asio.hpp>
#include <asio/ip/tcp.hpp>
#include <cstddef>
#include <functional>
#include "../AsioServer.hpp"
#include "../BuffSize.hpp"

#include <mutex>
#include <unordered_map>

using asio::ip::tcp;

namespace ntw {

class Session : public std::enable_shared_from_this<Session> {
    public:
    Session(tcp::socket &&sock, std::mutex &serverMutex, const std::function<void(std::size_t)> &closeHandler)
        : _sock(std::move(sock)), _serverMutex(serverMutex), _closeHandler(closeHandler), _id(_sock.native_handle())
    {
    }

    virtual ~Session() = default;

    void handleClient(std::function<void(tcp::socket &, char *, std::size_t)> &handler);

    tcp::socket &socket()
    {
        return _sock;
    }

    std::size_t &id()
    {
        return _id;
    }

    private:
    void _handleRead(
        asio::error_code ec,
        std::size_t bytes,
        std::function<void(tcp::socket &, char *, std::size_t)> &handler
    );

    tcp::socket _sock;
    std::mutex &_serverMutex;
    std::array<char, BUFF_SIZE> _buff;
    const std::function<void(std::size_t)> _closeHandler;
    std::size_t _id;
};

class TCPServer : public ntw::AsioServer {
    public:
    TCPServer(int port);
    ~TCPServer() override;

    void run() override;

    void registerCommand(std::function<void(tcp::socket &, char *, std::size_t)> func);
    void registerDisconnectionHandler(std::function<void(std::size_t)> func);

    void sockWrite(tcp::socket &sock, const char *data, std::size_t size);

    void removeUser(std::size_t id);

    void addUser(tcp::socket &sock, size_t id);

    void sendToUser(size_t id, const char *data, std::size_t size);

    void sendToAllUser(const char *data, std::size_t size);

    private:
    void _asioRun() override;

    void _handleAccept(asio::error_code ec, const std::shared_ptr<Session> &session);

    std::thread _thread;
    asio::io_context _io;
    tcp::acceptor _acc;
    std::function<void(tcp::socket &, char *, std::size_t)> _handler;
    std::function<void(std::size_t)> _disconnectionHandler;
    std::mutex _mutex;
    std::unordered_map<size_t, std::shared_ptr<Session>> _session;
};
} // namespace ntw
