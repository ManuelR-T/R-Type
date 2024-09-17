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
#include <vector>

void server::UDPServer::UDPServer::handle_recv(asio::error_code ec, std::size_t bytes)
{
    std::cout << "Received: [";
    std::cout.write(buff_, bytes - 1);
    std::cout << "]\n";

    if (ec) {
        std::cout << ec << std::endl;
        return;
    }
    if (bytes) {
        try {
            auto func = handlers_.find(std::string(buff_, bytes - 1));

            if (func == handlers_.end()) {
                throw std::runtime_error("No such command");
            }

            func->second(buff_, bytes);
        } catch (std::exception &err) {
            std::cerr << "No such command: [";
            std::cerr.write(buff_, bytes);
            std::cerr << "]\n";
        }
    }

    asio_run();
}

void server::UDPServer::UDPServer::handle_send(
    const std::vector<char> &vect
)
{
    sock_.async_send_to(
        asio::buffer(vect),
        endpoint_,
        [] (asio::error_code, std::size_t bytes) { std::cout << "I sent " << bytes << " Bytes\n"; }
    );
}

void server::UDPServer::UDPServer::register_command(
    char const *name,
    std::function<void (char *, std::size_t)> func)
{
    handlers_[name] = func;
}

void server::UDPServer::UDPServer::asio_run()
{
    std::cout << "Start Receive !\n";
    sock_.async_receive_from(
        asio::buffer(buff_), endpoint_,
        std::bind(
            &UDPServer::handle_recv,
            this,
            asio::placeholders::error,
            asio::placeholders::bytes_transferred
        )
    );
}

void server::UDPServer::UDPServer::run()
{
    asio_run();
    io_.run();
    std::cout << "End !\n";
}
