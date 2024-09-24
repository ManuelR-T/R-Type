/*
** EPITECH PROJECT, 2024
** network
** File description:
** TCPServer
*/

#include "TCPServer.hpp"

#include <asio/placeholders.hpp>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>
#include <asio/completion_condition.hpp>
#include <asio/error_code.hpp>

/*
    Session
*/

void server::Session::Session::handle_read(
    asio::error_code ec,
    std::size_t bytes,
    std::function<void(tcp::socket &, char *, std::size_t)> &handler
)
{
    if (ec) {
        std::cout << ec << std::endl;
        return;
    }
    if (bytes) {
        try {
            handler(sock_, buff_.data(), bytes);
        } catch (std::exception &err) {
            std::cerr << "No such command: ";
            std::cerr.write(buff_.data(), bytes);
            std::cerr << std::endl;
        }
    }
    handle_client(handler);
}

void server::Session::Session::handle_client(std::function<void(tcp::socket &, char *, std::size_t)> &handler)
{
    std::cout << "Session start !\n";
    sock_.async_receive(
        asio::buffer(buff_, BUFF_SIZE),
        [that = this->shared_from_this(), &handler] (asio::error_code ec, std::size_t bytes) {
            that->handle_read(ec, bytes, handler);
        }
    );
}

/*
    TCPServer
*/

server::TCPServer::TCPServer(int port) : acc_(io_, tcp::endpoint(tcp::v4(), port)) {}

server::TCPServer::~TCPServer()
{
    if (thread_.joinable()) {
        io_.stop();
        thread_.join();
    }
}

void server::TCPServer::sock_write(tcp::socket &sock_, std::string str)
{
    sock_.async_write_some(asio::buffer(str), [](asio::error_code ec, size_t) {
        if (ec) {
            return;
        }
    });
}

void server::TCPServer::TCPServer::register_command(std::function<void(tcp::socket &, char *, std::size_t)> func)
{
    handler_ = std::move(func);
}

void server::TCPServer::handle_accept(asio::error_code ec, std::shared_ptr<server::Session> session)
{
    if (ec) {
        return;
    }
    session->handle_client(handler_);
    asio_run();
}

void server::TCPServer::TCPServer::asio_run()
{
    auto session = std::make_shared<Session>(io_);

    acc_.async_accept(
        session->socket(), [this, session] (asio::error_code ec) {
            std::cout << "Accepted\n";
            handle_accept(ec, session);
        }
    );
}

void server::TCPServer::TCPServer::run()
{
    thread_ = std::thread([this]() {
        asio_run();
        io_.run();
    });
}
