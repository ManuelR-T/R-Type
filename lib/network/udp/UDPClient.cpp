/*
** EPITECH PROJECT, 2024
** network
** File description:
** UDPServer
*/

#include "UDPClient.hpp"
#include <asio/placeholders.hpp>
#include <iostream>

client::UDPClient::UDPClient(const std::string &host, int port)
    : endpoint_(asio::ip::address::from_string(host), port), sock_(io_)
{
    sock_.open(udp::v4());
}

void client::UDPClient::asio_run()
{
    sock_.async_receive_from(asio::buffer(buff_), endpoint_, [this](auto &&PH1, auto &&PH2) {
        handle_recv(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
    });
}

void client::UDPClient::handle_recv(asio::error_code ec, std::size_t bytes)
{
    if (ec) {
        std::cerr << "Receive error: " << ec.message() << std::endl;
        return;
    }

    asio_run();
}

void client::UDPClient::send(const char *data, std::size_t size)
{
    sock_.async_send_to(asio::buffer(data, size), endpoint_, [](asio::error_code ec, std::size_t bytes) {
        if (!ec) {
            std::cout << "Sent " << bytes << " bytes" << std::endl;
        } else {
            std::cerr << "Send error: " << ec.message() << std::endl;
        }
    });
}

void client::UDPClient::run()
{
    recv_thread_ = std::thread([this]() {
        asio_run();
        io_.run();
        std::cout << "Client terminated" << std::endl;
    });
}
