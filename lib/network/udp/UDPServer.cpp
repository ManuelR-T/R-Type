/*
** EPITECH PROJECT, 2024
** network
** File description:
** UDPServer
*/

#include "UDPServer.hpp"
#include <asio/placeholders.hpp>
#include <cstddef>
#include <iostream>

void ntw::UDPServer::_handleRecv(asio::error_code ec, std::size_t bytes)
{
    if (ec) {
        std::cout << ec << std::endl;
        return;
    }
    if (bytes) {
        _handler(_buff.data(), bytes);
    }

    _asioRun();
}

void ntw::UDPServer::handleSend(const char *data, std::size_t size)
{
    _sock.async_send_to(asio::buffer(data, size), _endpoint, [](asio::error_code, std::size_t bytes) {
        std::cout << "I sent data Bytes\n";
    });
}

void ntw::UDPServer::registerCommand(std::function<void(char *, std::size_t)> func)
{
    _handler = std::move(func);
}

void ntw::UDPServer::_asioRun()
{
    _sock.async_receive_from(asio::buffer(_buff), _endpoint, [this](auto &&pH1, auto &&pH2) {
        _handleRecv(std::forward<decltype(pH1)>(pH1), std::forward<decltype(pH2)>(pH2));
    });
}

void ntw::UDPServer::run()
{
    _recvThread = std::thread([this]() {
        _asioRun();
        _io.run();
    });
}
