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

void server::UDPServer::handle_recv(asio::error_code ec, std::size_t bytes)
{
    if (ec) {
        std::cout << ec << std::endl;
        return;
    }
    if (bytes) {
        handler_(buff_.data(), bytes);
    }

    asio_run();
}

void server::UDPServer::handle_send(const char *data, std::size_t size)
{
    sock_.async_send_to(asio::buffer(data, size), endpoint_, [](asio::error_code, std::size_t bytes) {
        std::cout << "I sent data Bytes\n";
    });
}

void server::UDPServer::register_command(std::function<void(char *, std::size_t)> func)
{
    handler_ = func;
}

void server::UDPServer::asio_run()
{
    sock_.async_receive_from(asio::buffer(buff_), endpoint_, [this](auto &&PH1, auto &&PH2) {
        handle_recv(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
    });
}

void server::UDPServer::run()
{
    recv_thread_ = std::thread([this]() {
        asio_run();
        io_.run();
        std::cout << "End !\n";
    });
}
