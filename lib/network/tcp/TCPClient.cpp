/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** TCPClient
*/

#include "TCPClient.hpp"
#include <functional>
#include <iostream>
#include <utility>
#include "../../utils/Logger.hpp"
#include <asio/ip/address_v4.hpp>
#include <asio/socket_base.hpp>

ntw::TCPClient::TCPClient(const std::string &host, int port) : _socket(_io), _host(host), _port(port)
{
    auto end = tcp::endpoint();
    _socket.connect(tcp::endpoint(asio::ip::make_address(host), _port));
}

ntw::TCPClient::~TCPClient()
{
    if (_thread.joinable()) {
        _io.stop();
        _thread.join();
    }
    if (_socket.is_open()) {
        _socket.shutdown(asio::socket_base::shutdown_both);
        _socket.close();
    }
}

void ntw::TCPClient::run()
{
    _thread = std::thread([this]() {
        _asioRun();
        _io.run();
        eng::logInfo("Client closed.");
    });
}

void ntw::TCPClient::send(const char *data, std::size_t size)
{
    _socket.async_write_some(asio::buffer(data, size), [](const asio::error_code &ec, std::size_t bytes) {
        if (ec) {
            eng::logWarning(ec.message());
        } else {
            eng::logInfo("Send " + std::to_string(bytes) + " bytes to server.");
        }
    });
}

void ntw::TCPClient::registerHandler(std::function<void(const char *, std::size_t)> handler)
{
    _recvHandler = std::move(handler);
}

void ntw::TCPClient::_asioRun()
{
    _socket.async_receive(asio::buffer(_buff, _buff.size()), [&](const asio::error_code &ec, std::size_t bytes) {
        if (!ec) {
            eng::logInfo("Recv " + std::to_string(bytes) + " bytes from server.");
            this->_recvHandler(_buff.data(), bytes);
            _asioRun();
        } else {
            std::cerr << ec.message() << std::endl;
        }
    });
}
