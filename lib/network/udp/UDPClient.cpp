/*
** EPITECH PROJECT, 2024
** network
** File description:
** UDPServer
*/

#include "UDPClient.hpp"
#include <asio/placeholders.hpp>
#include <iostream>
#include <vector>

client::UDPClient::UDPClient(const std::string &host, int port)
    : endpoint_(asio::ip::address::from_string(host), port),
      sock_(io_)
{
    sock_.open(udp::v4());
}

void client::UDPClient::asio_run()
{
    std::cout << "Start receiving data from server!" << std::endl;
    sock_.async_receive_from(
        asio::buffer(buff_), endpoint_,
        std::bind(
            &UDPClient::handle_recv,
            this,
            std::placeholders::_1,
            std::placeholders::_2
        )
    );
}

void client::UDPClient::handle_recv(asio::error_code ec, std::size_t bytes)
{
    if (ec) {
        std::cerr << "Receive error: " << ec.message() << std::endl;
        return;
    }

    // std::cout << "Received: ";
    // std::cout.write(buff_.data(), bytes);
    // std::cout << std::endl;

    asio_run();
}

void client::UDPClient::send(const char *data, std::size_t size)
{
    sock_.async_send_to(
        asio::buffer(data, size),
        endpoint_,
        [] (asio::error_code ec, std::size_t bytes) {
            if (!ec) {
                std::cout << "Sent " << bytes << " bytes" << std::endl;
            } else {
                std::cerr << "Send error: " << ec.message() << std::endl;
            }
        }
    );
}

void client::UDPClient::run()
{
    asio_run();
    io_.run();
    std::cout << "Client terminated" << std::endl;
}
