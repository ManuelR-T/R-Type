/*
** EPITECH PROJECT, 2024
** network
** File description:
** TCPServer
*/

#include "TCPServer.hpp"

#include <asio/completion_condition.hpp>
#include <asio/error_code.hpp>
#include <asio/placeholders.hpp>
#include <exception>
#include <iostream>
#include <memory>
#include <functional>
#include <stdexcept>

/*
    Session
*/

void server::Session::Session::handle_read(
    asio::error_code ec,
    std::size_t bytes,
    const std::map<std::string, std::function<void (char *, std::size_t)>> &handler
)
{
    if (ec) {
        std::cout << ec << std::endl;
        return;
    }
    if (bytes) {
        try {
            auto func = handler.find(std::string(buff_, bytes - 1));

            if (func == handler.end()) {
                throw std::runtime_error("No such command");
            }
            func->second(buff_, bytes);
        } catch (std::exception &err) {
            std::cerr << "No such command: ";
            std::cerr.write(buff_, bytes);
            std::cerr << std::endl;
        }
    }
    handle_client(handler);
}

void server::Session::Session::handle_client(
    const std::map<std::string, std::function<void (char *, std::size_t)>> &handler
)
{
    sock_.async_read_some(
        asio::buffer(buff_, BUFF_SIZE),
        std::bind(
            &Session::handle_read,
            this->shared_from_this(),
            asio::placeholders::error,
            asio::placeholders::bytes_transferred,
            std::ref(handler)
        )
    );
}

/*
    TCPServer
*/

server::TCPServer::TCPServer(int port):
    acc_(io_, tcp::endpoint(tcp::v4(), port))
{
}

void server::TCPServer::sock_write(tcp::socket &sock_, std::string str)
{
    sock_.async_write_some(
        asio::buffer(str),
        [] (asio::error_code ec, size_t) {
            if (ec) {
                return;
            }
        }
    );
}

void server::TCPServer::TCPServer::register_command(
    char const *name,
    std::function<void (char *, std::size_t)> func)
{
    handlers_[name] = func;
}

void server::TCPServer::handle_accept(asio::error_code ec, std::shared_ptr<server::Session> conn)
{
    if (ec) {
        return;
    }
    conn->handle_client(handlers_);
    asio_run();
}

void server::TCPServer::TCPServer::asio_run()
{
    auto session = std::make_shared<Session>(io_);

    acc_.async_accept(
        session->socket(),
        std::bind(
            &TCPServer::handle_accept,
            this,
            asio::placeholders::error,
            session
        )
    );
}

void server::TCPServer::TCPServer::run()
{
    asio_run();
    io_.run();
}

