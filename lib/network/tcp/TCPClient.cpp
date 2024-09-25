/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** TCPClient
*/

#include "TCPClient.hpp"
#include <cstddef>
#include <iostream>
#include <asio/ip/address_v4.hpp>

client::TCPClient::TCPClient(const std::string &host, int port) : socket_(io_), host_(host), port_(port)
{
    auto end = tcp::endpoint();
    socket_.connect(tcp::endpoint(asio::ip::address::from_string(host.c_str()), port_));
}

client::TCPClient::~TCPClient()
{
    if (thread_.joinable()) {
        io_.stop();
        thread_.join();
    }
}

void client::TCPClient::send(const char *data, std::size_t size)
{
    socket_.async_write_some(asio::buffer(data, size), [](const asio::error_code &ec, std::size_t bytes) {
        std::cout << "Message sent !" << std::endl;
    });
}

void client::TCPClient::asio_run()
{
    std::cout << "Start receiving data from server!" << std::endl;
    socket_.async_receive(asio::buffer(buff_, buff_.size()), [&](const asio::error_code &ec, std::size_t bytes) {
        if (!ec) {
            this->recv_handler_(buff_.data(), bytes);
            asio_run();
        }
    });
}

void client::TCPClient::run()
{
    thread_ = std::thread([this]() {
        asio_run();
        io_.run();
        std::cout << "Client terminated" << std::endl;
    });
}

void client::TCPClient::register_handler(std::function<void(const char *, std::size_t)> handler)
{
    recv_handler_ = std::move(handler);
}
