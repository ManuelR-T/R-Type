/*
** EPITECH PROJECT, 2024
** network
** File description:
** UDPServer
*/

#include "UDPClient.hpp"
#include <asio/placeholders.hpp>
#include <iostream>

ntw::UDPClient::UDPClient(const std::string &host, int port)
    : _endpoint(asio::ip::address::from_string(host), port), _sock(_io)
{
    _sock.open(udp::v4());
}

void ntw::UDPClient::_asioRun()
{
    _sock.async_receive_from(asio::buffer(_buff), _endpoint, [this](auto &&pH1, auto &&pH2) {
        _handleRecv(std::forward<decltype(pH1)>(pH1), std::forward<decltype(pH2)>(pH2));
    });
}

void ntw::UDPClient::_handleRecv(asio::error_code ec, std::size_t bytes)
{
    if (ec) {
        std::cerr << "Receive error: " << ec.message() << std::endl;
        return;
    }

    _asioRun();
}

void ntw::UDPClient::send(const char *data, std::size_t size)
{
    _sock.async_send_to(asio::buffer(data, size), _endpoint, [](asio::error_code ec, std::size_t bytes) {
        if (!ec) {
            std::cout << "Sent " << bytes << " bytes" << std::endl;
        } else {
            std::cerr << "Send error: " << ec.message() << std::endl;
        }
    });
}

void ntw::UDPClient::run()
{
    _recvThread = std::thread([this]() {
        _asioRun();
        _io.run();
        std::cout << "Client terminated" << std::endl;
    });
}
